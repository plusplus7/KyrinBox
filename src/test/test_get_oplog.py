# -*- coding: utf-8 -*-
import urllib
import sys
import urllib2
import get_oplog_pb2 
import upload_file_pb2
import operation_log_pb2
import base64

url = "http://localhost:17070/GetOplog"
def main():
    post_data = [chr(0), chr(0), chr(0), chr(0), chr(0), chr(0), chr(0), chr(0)]
    post_data = base64.b64encode("".join(post_data))
    request = urllib2.Request(url, post_data)

    response = urllib2.urlopen(request)
    a = response.read()
    ste = base64.b64decode(a)
    get_oplog_response = get_oplog_pb2.GetOplogResponse()
    get_oplog_response.ParseFromString(ste)
    print get_oplog_response.log_data

    for i in range(len(get_oplog_response.last_id)):
        print ord(get_oplog_response.last_id[i]),
    print ''
    for i in get_oplog_response.log_data:
        print "### i ###"
        print i
        print "### i ###"
        op = operation_log_pb2.OperationLog()
        op.ParseFromString(i)
        print op
        print "### op ###"
        res = upload_file_pb2.UploadFileOperation()
        res.ParseFromString(op.log_data)
        print res
        print "### op ###"

if __name__ == "__main__":
    main()
