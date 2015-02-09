#include "common/kyrin_log.h"
#include "common/kyrin_constants.h"
#include "common/kyrin_macros.h"
#include "common/kyrin_lexicographically_helper.h"
#include "common/crypto/kyrin_base64.h"
#include "upload_file_request_handler.h"

namespace kyrin {
namespace server {

using namespace std;
using namespace kyrin::common;

static KyrinLog *logger = KyrinLog::get_instance();
void ConfirmOplogRequestHandler::handle_request(KyrinMasterServer *server, evhttp_request *req)
{
    KyrinMasterStatus status;
    string reply = "System error";

    kyrin_todo();

    if (!m_sentinel->get_status(status)) {
        reply = "Service denied";
        break;
    }
    cout << "not here" << endl;
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
    m_state_id = "";
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
            reply = "no ok";
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
