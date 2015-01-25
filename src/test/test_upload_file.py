import urllib
import urllib2
import upload_file_pb2

url = "http://localhost:7771/UploadFile"
def main():
    upload_file_request = upload_file_pb2.UploadFileRequest()
    upload_file_request.account        = "plusplus7"
    upload_file_request.file_name      = "sssogs"
    upload_file_request.merkle_sha1    = "sha1"
    upload_file_request.file_size      = 1024
    post_data = upload_file_request.SerializeToString()
    request = urllib2.Request(url, post_data)
    request.add_header('KYRIN-TIMESTAMP', "19930916")
    request.add_header('KYRIN-SIGNATURE', "signature")

    response = urllib2.urlopen(request)
    print response.read()


if __name__ == "__main__":
    main()
