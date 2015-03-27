import urllib
import urllib2
import upload_chunk_file_pb2
import test_sdk
import base64

def main():
    fp = open("a.out", "rb+")
    help(fp)
    arr = []
    while True:
        pr = fp.tell()
        tmp = fp.read(1024)
        if pr == fp.tell():
            break
        arr.append(tmp)

    idx = 0
    for i in arr:
        ucfr = upload_chunk_file_pb2.UploadChunkFileRequest()
        ucfr.account        = "plusplus7"
        ucfr.file_name      = "sssogs103"
        ucfr.offset         = idx*1024
        idx = idx+1
        ucfr.chunk          = base64.b64encode(i)
        headers = {'KYRIN-TIMESTAMP' : "19930916",
                   'KYRIN-SIGNATURE' : "signature"}

        print test_sdk.UploadChunkFile("localhost", 37077, ucfr, headers)


if __name__ == "__main__":
    main()
