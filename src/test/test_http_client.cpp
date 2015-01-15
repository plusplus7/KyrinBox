#include "io/kyrin_http_client.h"
#include <iostream>
#include <string>

using namespace std;
using namespace kyrin::io;

void http_request_done(struct evhttp_request *req, void *arg){
    char buf[1024];
    int len = evbuffer_remove(req->input_buffer, &buf, sizeof(buf) - 1);

    buf[len] = '\0';
    cout<<buf<<endl;
    event_base_loopbreak((event_base*)arg);
}
int main() {
    KyrinHttpClient::make_ev_request_get(http_request_done, "127.0.0.1", 7777, "/get_status");
    string response;
    KyrinHttpClient::make_request_get("127.0.0.1", 7777, "/get_status", response);
    cout<<response<<endl;
}
