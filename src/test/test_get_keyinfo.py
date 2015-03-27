import test_sdk
import upload_chunk_file_pb2 as ucf

def main():
    request = ucf.GetFileKeyInfo()
    request.account         = "plusplus7"
    request.file_name       = "sssogs"
    res = test_sdk.GetFileKeyinfo("127.0.0.1", 37370, request, {})
    print res[1]

if __name__ == "__main__":
    main()
