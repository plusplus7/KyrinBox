import urllib
import urllib2
import upload_file_pb2
import test_sdk

def main():
    upload_file_request = upload_file_pb2.UploadFileRequest()
    upload_file_request.account        = "plusplus7"
    upload_file_request.file_name      = "sssogs100"
    upload_file_request.merkle_sha1    = "sha1"
    upload_file_request.file_size      = 1024
    headers = {'KYRIN-TIMESTAMP' : "19930916",
               'KYRIN-SIGNATURE' : "signature"}

    (code, upload_file_response) = test_sdk.UploadFile("localhost", 27770, upload_file_request, headers)
    if code != 200:
        print upload_file_response
    else:
        print "1.", upload_file_response.file_hosts
        print "2.", upload_file_response.file_size
        print "3.", upload_file_response.merkle_sha1

if __name__ == "__main__":
    main()
