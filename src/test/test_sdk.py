import urllib
import urllib2
import sys
import get_oplog_pb2
import upload_file_pb2
import operation_log_pb2
import upload_chunk_file_pb2
import kyrin_key_pb2
import base64

def LexicographicallyGetZero():
    id = "".join([chr(0), chr(0), chr(0), chr(0), chr(0), chr(0), chr(0), chr(0)])
    return id

def LexicographicallyAddOne(id):
    t = list(id)
    for i in xrange(7, -1, -1):
        if ord(t[i]) != 255:
            t[i] = chr(ord(t[i])+1)
            break
    id = "".join(t)
    return id

def GetOplog(host, port, op_id = None):
    url = "http://%s:%d/GetOplog" % (host, port)
    if op_id == None:
        op_id = LexicographicallyGetZero()
    post_data = base64.b64encode("".join(op_id))
    request = urllib2.Request(url, post_data)
    response = ""
    try:
        response = urllib2.urlopen(request)
    except urllib2.HTTPError, e:
        response = e
        return (response.getcode(), response.read())
    get_oplog_response = get_oplog_pb2.GetOplogResponse()
    get_oplog_response.ParseFromString(base64.b64decode(response.read()))
    return (200, get_oplog_response)

def UploadFile(host, port, request, headers):
    url = "http://%s:%d/UploadFile" % (host, port)
    post_data = request.SerializeToString()
    request = urllib2.Request(url, post_data)
    for k, v in headers.items():
        request.add_header(k, v)
    try:
        response = urllib2.urlopen(request)
    except urllib2.HTTPError, e:
        response = e
        return (response.getcode(), response.read())
    upload_file_response = upload_file_pb2.UploadFileResponse()
    upload_file_response.ParseFromString(response.read())
    return (200, upload_file_response)

def FindLeader(host, ports):
    url = "http://%s:%d/get_status"
    ret = None
    for i in range(len(ports)):
        request = urllib2.Request(url % (host, ports[i]))
        try:
            response = urllib2.urlopen(request)
        except urllib2.HTTPError, e:
            response = e
            return (response.getcode(), response.read())
        get_status_response = response.read()
        if cmp(get_status_response, "leader") == 0:
            if ret == None:
                ret = (host, i)
            else:
                return None
    return ret

def SetFileKeyinfo(host, port, request, headers):
    url = "http://%s:%d/SetFileKeyInfo" % (host, port)
    ret = None
    post_data = request.SerializeToString()
    post_data = base64.b64encode(post_data)
    request = urllib2.Request(url, post_data)
    for k, v in headers.items():
        request.add_header(k, v)
    try:
        response = urllib2.urlopen(request)
    except urllib2.HTTPError, e:
        response = e
        return (response.getcode(), response.read())
    return (200, response.read())

def GetFileKeyinfo(host, port, request, headers):
    url = "http://%s:%d/GetFileKeyInfo" % (host, port)
    post_data = request.SerializeToString()
    post_data = base64.b64encode(post_data)
    request = urllib2.Request(url, post_data)
    for k, v in headers.items():
        request.add_header(k, v)
    try:
        response = urllib2.urlopen(request)
    except urllib2.HTTPError, e:
        response = e
        return (response.getcode(), response.read())
    get_file_key_info_response = upload_chunk_file_pb2.SetFileKeyInfo()
    get_file_key_info_response.ParseFromString(base64.b64decode(response.read()))
    return (200, get_file_key_info_response)

def UploadChunkFile(host, port, request, headers):
    url = "http://%s:%d/UploadChunkFile" % (host, port)
    post_data = request.SerializeToString()
    post_data = base64.b64encode(post_data)
    request = urllib2.Request(url, post_data)
    for k, v in headers.items():
        request.add_header(k, v)
    try:
        response = urllib2.urlopen(request)
    except urllib2.HTTPError, e:
        response = e
        return (response.getcode(), response.read())
    return (200, response.read())

def DownloadChunkFile(host, port, request, headers):
    url = "http://%s:%d/DownloadChunkFile" %(host, port)
    post_data = request.SerializeToString()
    post_data = base64.b64encode(post_data)
    request = urllib2.Request(url, post_data)
    for k, v in headers.items():
        request.add_header(k, v)
    try:
        response = urllib2.urlopen(request)
    except urllib2.HTTPError, e:
        response = e
        return (response.getcode(), response.read())
    return (200, response.read())

def SetKyrinKey(host, port, postdata, headers):
    url = "http://%s:%d/SetKyrinKey" %(host, port)
    request = urllib2.Request(url, postdata)
    for k, v in headers.items():
        request.add_header(k, v)
    try:
        response = urllib2.urlopen(request)
    except urllib2.HTTPError, e:
        response = e
        return (response.getcode(), response.read())
    return (200, response.read())

if __name__ == "__main__":
    print FindLeader("127.0.0.1", (7777, 17777, 27777))
    id = LexicographicallyGetZero()
    for i in list(id):
        print ord(i),
    id = LexicographicallyAddOne(id)
    for i in list(id):
        print ord(i),
