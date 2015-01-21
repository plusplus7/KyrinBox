#include "kyrin_http_client.h"
#include <curl/curl.h>

namespace kyrin {
namespace io {

using namespace std;
/*
 * Warning: This class which for http requesting should called carefully
 * For event base would not break the loop automaticly, every callback for this use should call event_base_loopbreak  or resource blackhole
 */
void KyrinHttpClient::make_ev_request_get(void(*request_done_callback)(struct evhttp_request *, void *), const char *host, uint32_t port,  const char *uri, int timeout)
{
    event_base *base = event_base_new();
    evhttp_connection *connection = evhttp_connection_base_new(base, NULL, host, port);
    evhttp_request *request = evhttp_request_new(request_done_callback, base);

    evhttp_make_request(connection, request, EVHTTP_REQ_GET, uri);
    evhttp_connection_set_timeout(request->evcon, timeout);
    event_base_dispatch(base);
    return ;
}

static long write_function(void *data, int size, int nmemb, string &content)
{
    long sizes = size * nmemb;
    string temp((char *)data, sizes);
    content += temp; 
    return sizes;
}

bool KyrinHttpClient::make_request_get(const char *host, uint32_t port, const char *uri, string &response, int timeout)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        char url[1024];
        sprintf(url, "http://%s:%d%s", host, port, uri);
        res = curl_easy_setopt(curl, CURLOPT_URL, url);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }
    }

    curl_easy_cleanup(curl);
    return true;
}

bool KyrinHttpClient::make_request_post(const char *host, uint32_t port, const char *uri, string &response, string &postdata, int timeout)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        char url[1024];
        sprintf(url, "http://%s:%d%s", host, port, uri);
        res = curl_easy_setopt(curl, CURLOPT_URL, url);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        /*
        res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }
        */

        res = curl_easy_setopt(curl, CURLOPT_POST, 1);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "";
            return false;
        }
    }

    curl_easy_cleanup(curl);
    return true;
}

} /* io */
} /* kyrin */
