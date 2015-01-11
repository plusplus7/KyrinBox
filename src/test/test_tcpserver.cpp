#include "io/kyrin_network.h"
#include <vector>
#include <iostream>
#include <utility>

using namespace std;
using namespace kyrin::io;

int main()
{
    TcpServerWrapper wrapper;

    vector< pair<uint32_t, int> > clients;
    clients.push_back(make_pair(inet_addr("127.0.0.1"), 1));
    clients.push_back(make_pair(inet_addr("127.0.0.1"), 2));
    clients.push_back(make_pair(inet_addr("127.0.0.1"), 3));
    wrapper.initialize(20000, clients);

    cout<< "connect finished" << endl;
    string msg = "ahahaha";
    for (int i=0; i<10; i++) {
        for (int j=0; j<clients.size(); j++) {
            cout << i << " " << j << " ";
            cout << wrapper.send_message(clients[j].second, msg) << endl;
        }
        sleep(1);
    }
}
