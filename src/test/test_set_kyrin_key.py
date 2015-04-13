import test_sdk
import hashlib
import kyrin_key_pb2
from M2Crypto import RSA, X509
import base64

def main():
    fp = open("../deploy/cert.pem", "r")
    set_key_request = kyrin_key_pb2.SetKyrinKeyRequest()
    set_key_request.key_id      = "TestKyrinKey"
    set_key_request.key_public  = fp.read() 
    postdata = set_key_request.SerializeToString()
    postdata = base64.b64encode(postdata)
    headers = {}
    headers["KYRIN-TIMESTAMP"] = "1990"

    digest = hashlib.sha1("1990"+postdata).hexdigest()
    print digest 
    fp = open("../deploy/key.pem", "r")
    key = RSA.load_key_string(fp.read())
    headers["KYRIN-SIGNATURE"] = base64.b64encode(key.private_encrypt(digest, RSA.pkcs1_padding))
    print headers["KYRIN-SIGNATURE"]
    print test_sdk.SetKyrinKey("127.0.0.1", 7086, postdata, headers)



if __name__ == "__main__":
    main()
