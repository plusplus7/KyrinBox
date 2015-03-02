# -*- coding: utf-8 -*-
import urllib
import sys
import urllib2
import get_oplog_pb2 
import upload_file_pb2
import operation_log_pb2
import base64
import test_sdk

def main(port):
    print "###########"
    print "testing %d" % (port)
    print "###########"
    (code, get_oplog_response) = test_sdk.GetOplog("localhost", port)
    if code != 200:
        print get_oplog_response
        return

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
    main(17070)
    main(27070)
    main(37070)
