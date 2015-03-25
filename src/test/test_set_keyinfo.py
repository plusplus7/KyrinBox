import test_sdk
import upload_chunk_file_pb2 as ucf

def main():
    request = ucf.SetFileKeyInfo()
    request.account         = "plusplus7"
    request.file_name       = "sssogs"
    request.file_hosts.append("2332323")
    request.file_size       = 233
    request.merkle_sha1     = "sha1"
    request.key_aes         = "aes"
    print test_sdk.SetFileKeyinfo("127.0.0.1", 37270, request, {})
if __name__ == "__main__":
    main()
