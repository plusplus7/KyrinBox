#include <unistd.h>
#include "kyrin_chunk_server.h"
#include "common/configs/kyrin_chunk_config.h"
#include "common/kyrin_cluster.h"

namespace kyrin {
namespace server {

using namespace kyrin::common;

void KyrinChunkServer::process_upload_chunk_file_request(evhttp_request *req)
{
    if (!m_examine_identity_request_filter->filter_request(this, req))
        return ;
    m_upload_chunk_file_request_handler->handle_request(this, req);
}

void KyrinChunkServer::process_download_chunk_file_request(evhttp_request *req)
{
    if (!m_examine_identity_request_filter->filter_request(this, req))
        return ;
    m_download_chunk_file_request_handler->handle_request(this, req);
}

void KyrinChunkServer::process_set_file_key_info_request(evhttp_request *req)
{
    if (!m_examine_identity_request_filter->filter_request(this, req))
        return ;
    m_set_file_key_info_request_handler->handle_request(this, req);
}

void KyrinChunkServer::process_get_file_key_info_request(evhttp_request *req)
{
    if (!m_examine_identity_request_filter->filter_request(this, req))
        return ;
    m_get_file_key_info_request_handler->handle_request(this, req);
}

static void upload_chunk_file_handler(evhttp_request *req, void *arg)
{
    ((KyrinChunkServer *) arg)->process_upload_chunk_file_request(req);
}

static void download_chunk_file_handler(evhttp_request *req, void *arg)
{
    ((KyrinChunkServer *) arg)->process_download_chunk_file_request(req);
}

static void get_file_key_info_handler(evhttp_request *req, void *arg)
{
    ((KyrinChunkServer *) arg)->process_get_file_key_info_request(req);
}

static void set_file_key_info_handler(evhttp_request *req, void *arg)
{
    ((KyrinChunkServer *) arg)->process_set_file_key_info_request(req);
}

bool KyrinChunkServer::server_initialize(KyrinChunkGossiper *gossiper)
{
    if (gossiper == NULL) {
        return false;
    }
    m_gossiper = gossiper;

    configs::KyrinChunkConfig *config = new configs::KyrinChunkConfig();
    config->read_config_file((char *)"kyrinbox_config.json");
    if (!server_initialize_kyrin_server_socket(upload_chunk_file_fd,
         config->upload_chunk_file_port(),
         config->upload_chunk_file_backlog())) {
         delete config;
         return false;
    }

    if (!server_initialize_kyrin_server_socket(download_chunk_file_fd,
         config->download_chunk_file_port(),
         config->download_chunk_file_backlog())) {
         delete config;
         return false;
    }

    if (!server_initialize_kyrin_server_socket(set_file_key_info_fd,
         config->set_file_key_info_port(),
         config->set_file_key_info_backlog())) {
         delete config;
         return false;
    }

    if (!server_initialize_kyrin_server_socket(get_file_key_info_fd,
         config->get_file_key_info_port(),
         config->get_file_key_info_backlog())) {
         delete config;
         return false;
    }

    m_keyinfo_db = new io::KyrinDatabaseWrapper(KyrinCluster::get_instance()->get_chunk_config()->keyinfo_database_path().c_str());
    m_upload_chunk_file_request_handler = new UploadChunkFileRequestHandler(m_gossiper);
    m_download_chunk_file_request_handler = new DownloadChunkFileRequestHandler();
    m_set_file_key_info_request_handler = new SetFileKeyInfoRequestHandler(m_keyinfo_db);
    m_get_file_key_info_request_handler = new GetFileKeyInfoRequestHandler(m_keyinfo_db);
    m_examine_identity_request_filter   = new ExamineIdentityRequestFilter(KyrinCluster::get_instance()->get_keycenter_host(), KyrinCluster::get_instance()->get_keycenter_get_key_port());
    delete config;
    return true;
}

bool KyrinChunkServer::server_free()
{
    KyrinBaseServer::server_free();
    close(upload_chunk_file_fd);
    delete m_upload_chunk_file_request_handler;
    delete m_download_chunk_file_request_handler;
    delete m_set_file_key_info_request_handler;
    delete m_get_file_key_info_request_handler;
    delete m_keyinfo_db;
    delete m_examine_identity_request_filter;
    return true;
}

bool KyrinChunkServer::server_start()
{
    server_run(4);
    return true;
}

bool KyrinChunkServer::server_set_processor(evhttp *server, int thread_code)
{
    if (thread_code == 0) {
        server_put_callback(server, "/UploadChunkFile", upload_chunk_file_handler, this);
        server_set_evhttp_accept_socket(server, upload_chunk_file_fd);
    } else if (thread_code == 1) {
        server_put_callback(server, "/DownloadChunkFile", download_chunk_file_handler, this);
        server_set_evhttp_accept_socket(server, download_chunk_file_fd);
    } else if (thread_code == 2) {
        server_put_callback(server, "/SetFileKeyinfo", set_file_key_info_handler, this);
        server_set_evhttp_accept_socket(server, set_file_key_info_fd);
    } else {
        server_put_callback(server, "/GetFileKeyinfo", get_file_key_info_handler, this);
        server_set_evhttp_accept_socket(server, get_file_key_info_fd);
    }
    return true;
}

} /* server */
} /* kyrin */
