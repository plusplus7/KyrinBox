import urllib
import urllib2
import upload_chunk_file_pb2
import test_sdk
import base64

def main():
    fp = open("b.out", "rb+")
    idx = 0
    while True:
        ucfr = upload_chunk_file_pb2.DownloadChunkFileRequest()
        ucfr.account        = "plusplus7"
        ucfr.file_name      = "sssogs103"
        ucfr.offset         = idx*1024
        idx = idx+1
        headers = {'KYRIN-TIMESTAMP' : "19930916",
                   'KYRIN-SIGNATURE' : "signature"}

        res = test_sdk.DownloadChunkFile("localhost", 37477, ucfr, headers)
        print res
        tmp = res[1]
        if len(res[1]) == 0:
            break
        raw_input()
        tmp = base64.b64decode(tmp)
        fp.write(tmp)
    fp.close()


if __name__ == "__main__":
    main()
