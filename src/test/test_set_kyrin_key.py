import test_sdk
import kyrin_key_pb2

def main():
    set_key_request = kyrin_key_pb2.SetKyrinKeyRequest()
    set_key_request.key_id      = ""
    set_key_request.key_public  = ""
