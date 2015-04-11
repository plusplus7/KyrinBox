import test_sdk
import kyrin_key_pb2
import base64

def main():
    fp = open("cert.pem", "r")
    set_key_request = kyrin_key_pb2.SetKyrinKeyRequest()
    set_key_request.key_id      = "TestKyrinKey"
    set_key_request.key_public  = fp.read() 

    print test_sdk.SetKyrinKey("localhost", 7086, )
