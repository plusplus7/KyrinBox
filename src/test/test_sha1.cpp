#include "common/crypto/kyrin_sha1.h"
#include <cstdio>
#include <string>

using namespace std;
using namespace kyrin::common;

int main()
{
    string a = "ahahahah";
    string res = "";
    crypto::kyrin_sha1(a, res);
    for (int i=0; i<res.length(); i++) {
        printf("%x", (unsigned char)res[i]);
    }
    printf("\n");
    return 0;
}
