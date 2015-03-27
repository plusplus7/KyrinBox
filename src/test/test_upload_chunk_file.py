import urllib
import urllib2
import upload_chunk_file_pb2
import test_sdk
import base64

def main():
    fp = open("a.out", "rb+")
    help(fp)
    one = fp.read(500)
    fp.seek(500)
    two = fp.read()
    ucfr = upload_chunk_file_pb2.UploadChunkFileRequest()
    ucfr.account        = "plusplus7"
    ucfr.file_name      = "sssogs103"
    ucfr.offset         = 0
    ucfr.chunk          = base64.b64encode(one)
    headers = {'KYRIN-TIMESTAMP' : "19930916",
               'KYRIN-SIGNATURE' : "signature"}

    print test_sdk.UploadChunkFile("localhost", 37077, ucfr, headers)
    raw_input()
    ucfr = upload_chunk_file_pb2.UploadChunkFileRequest()
    ucfr.account        = "plusplus7"
    ucfr.file_name      = "sssogs103"
    ucfr.offset         = 500
    ucfr.chunk          = base64.b64encode(two)
    headers = {'KYRIN-TIMESTAMP' : "19930916",
               'KYRIN-SIGNATURE' : "signature"}

    print test_sdk.UploadChunkFile("localhost", 37077, ucfr, headers)


if __name__ == "__main__":
    main()
