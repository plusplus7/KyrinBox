#include "common/kyrin_log.h"
#include "io/kyrin_http_client.h"
#include "common/kyrin_constants.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_lexicographically_helper.h"
#include "common/crypto/kyrin_base64.h"
#include "upload_file_request_handler.h"
#include "protobuf/upload_file.pb.h"
#include "protobuf/get_oplog.pb.h"
#include "protobuf/operation_log.pb.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;
using namespace kyrin::io;

static KyrinLog *logger = KyrinLog::get_instance();
bool ConfirmOplogRequestHandler::update_oplog(string &last_id, string &target_id)
{
    uint32_t leader;
    m_sentinel->get_leader(leader);
    KyrinCluster *cluster = KyrinCluster::get_instance();
    string current_last_id;
    current_last_id.assign(last_id);
    while (true) {
        string response;
        string to_post = crypto::base64_encode((unsigned char const*)last_id.c_str(), last_id.length());
        if (!KyrinHttpClient::make_request_post(cluster->get_master_ip(leader),
                                                cluster->get_master_get_oplog_port(leader),
                                                "/GetOplog",
                                                response,
                                                to_post)) {
            return false;
        }
        kyrinbox::api::GetOplogResponse data;
        data.ParseFromString(crypto::base64_decode(response));
        for (int i=0; i<data.log_data_size(); i++) {
            kyrinbox::api::OperationLog op_log;
            op_log.ParseFromString(data.log_data(i));
            if (op_log.op_type() == 1) {  // Upload file operation
                kyrinbox::api::UploadFileOperation op_data;
                op_data.ParseFromString(op_log.log_data());
                string operation_data;
                if (!op_data.data().SerializeToString(&operation_data)) {
                    logger->log("update_oplog", "Can't serialize op_data to string");
                    return false;
                }
                if (!m_oplog_db->put(current_last_id, data.log_data(i))) {
                    logger->log("update_oplog", "Can't put in oplog");
                    return false;
                }
                if (!m_userdata_db->put(op_data.key(), operation_data)) {
                    logger->log("update_oplog", "Can't put in userdata");
                    m_oplog_db->remove(current_last_id);
                    return false;
                }
                KyrinLexicographicallyHelper::add_one(current_last_id);
            }
        }
        if (current_last_id == target_id) {
            break;
        }
    }
    return true;
}

void ConfirmOplogRequestHandler::handle_request(KyrinMasterServer *server, evhttp_request *req)
{
    KyrinMasterStatus status;
    string reply = "System error";

    kyrin_todo();

    if (!m_sentinel->get_status(status)) {
        reply = "Service denied";
        break;
    }
    if (status == k_status_leader) {
        break;
    }
    string request_body = "";
    if (!server->server_get_postdata(req, request_body)) {
        reply = "Can't read post data.";
        break;
    }
    request_body = crypto::base64_decode(request_body);
    kyrinbox::api::OperationLogMessage message;
    if (!message.ParseFromString(request_body)) {
        reply = "Can't parse from post data";
        break;
    }
    uint32_t state = message.state();
    if (state == 1) {
        m_state_id = message.op_id();
        string last_id;
        if (kyrin_likely(m_oplog_db->fetch_last_key(last_id))) {
            KyrinLexicographicallyHelper::add_one(last_id);
        } else {
            KyrinLexicographicallyHelper::get_zero(last_id);
        }
        if (last_id == m_state_id) {
            m_state = true;
            m_state_data = message.op_log();
            reply = "ok";
            server->server_send_reply_ok(req, reply);
            return ;
        } else {
            if (update_oplog(last_id, m_state_id)) {
                m_state = true;
                m_state_data = message.op_log();
                reply = "ok";
            } else {
                reply = "no ok";
            }
            break;
        }
    } else if (state == 2) {
        if (!m_state) {
            break;
        }
        string current_id = message.op_id();
        if (current_id == m_state_id) {
            if (m_state_data.op_type() == 1) {    // upload_file
                kyrinbox::api::UploadFileOperation operation;
                operation.ParseFromString(m_state_data.log_data());
                string op_log_str;
                if (!m_state_data.SerializeToString(&op_log_str)) {
                    reply = "Serialize op_log failed";
                    break;
                }
                
                string operation_data;
                if (!operation.data().SerializeToString(&operation_data)) {
                    reply = "Serialize op_data failed";
                    break;
                }

                if (!m_oplog_db->put(current_id, op_log_str)) {
                    reply = "put in oplog failed";
                    break;
                }
                if (!m_userdata_db->put(operation.key(), operation_data)) {
                    reply = "put in userdata failed";
                    m_oplog_db->remove(current_id);
                    break;
                }
                reply = "ok";
                server->server_send_reply_ok(req, reply);
                return ;
            } else {
                reply = "No suck operation type...";
                break;
            }
        } else {
            reply = "No sucking current id";
            break;
        }
    }

    kyrin_done();
    server->server_send_reply_bad(req, reply);
}

} /* server */
} /* kyrin */
