import urllib
import urllib2
import sys
import time
import get_oplog_pb2
import upload_file_pb2
import operation_log_pb2
import upload_chunk_file_pb2
import chunk_cluster_status_pb2
import kyrin_key_pb2
import base64
import hashlib
import json
from M2Crypto import RSA, X509

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

class KyrinboxSdk:

    def __init__(self, conf_path, username, key_path):
        fp = open(conf_path, "r")
        self.config = json.load(fp)
        fp.close()
        fp = open(key_path, "r")
        self.key    = RSA.load_key_string(fp.read())
        fp.close()
        self.username = username

    def protobuf_to_postdata(self, protobuf):
        postdata = protobuf.SerializeToString()
        postdata = base64.b64encode(postdata)
        return postdata

    def do_request_post(self, server, index, api, postdata = None):
        timestamp = str(int(time.time()))
        digest = hashlib.sha1(timestamp+postdata).hexdigest()
        headers = {}
        headers["KYRIN-SIGNATURE"] = base64.b64encode(self.key.private_encrypt(digest, RSA.pkcs1_padding))
        headers["KYRIN-TIMESTAMP"] = timestamp
        headers["KYRIN-ISSUER"]    = self.username
        url = "http://%s:%d/%s" % (self.config[server][index]["Host"], self.config[server][index][api+"Port"], api)
        request = urllib2.Request(url, postdata)
        for k, v in headers.items():
            request.add_header(k, v)
        try:
            response = urllib2.urlopen(request)
        except urllib2.HTTPError, e:
            response = e
        return (response.getcode(), response.read())

    def do_request_get(self, server, index, api):
        url = "http://%s:%d/%s" % (self.config[server][index]["Host"], self.config[server][index][api+"Port"], api)
        request = urllib2.Request(url)
        try:
            response = urllib2.urlopen(request)
        except urllib2.HTTPError, e:
            response = e
        return (response.getcode(), response.read())

    def set_kyrin_key(self, key_id, key_pub):
        set_key_request = kyrin_key_pb2.SetKyrinKeyRequest()
        set_key_request.key_id      = key_id
        set_key_request.key_public   = key_pub
        postdata = self.protobuf_to_postdata(set_key_request)
        return self.do_request_post("KeyCenter", 0, "SetKyrinKey", postdata)

    def get_kyrin_key(self, key_id):
        get_key_request = kyrin_key_pb2.GetKyrinKeyRequest()
        get_key_request.key_id  = key_id
        postdata = self.protobuf_to_postdata(get_key_request)
        return self.do_request_post("KeyCenter", 0, "GetKyrinKey", postdata)

    def get_status_master(self):
        res = []
        for i in range(3):
            res.append(self.do_request_get("Master", i, "GetStatus"))
        return res

    def get_status_chunk(self, index):
        postdata = "Read"
        (httpcode, response) = self.do_request_post("Chunk", index, "GetStatus", postdata)
        if httpcode == 200:
            chunk_status = chunk_cluster_status_pb2.ChunkClusterStatus()
            chunk_status.ParseFromString(base64.b64decode(response))
            return (httpcode, chunk_status)
        else:
            return (httpcode, chunk_status)

    def get_index_by_kbid(self, kbid):
        for i in range(len(self.config["Master"])):
            if self.config["Master"][i]["Kbid"] == kbid:
                return i
        for i in range(len(self.config["Chunk"])):
            if self.config["Chunk"][i]["Kbid"] == kbid:
                return i

    def get_master_leader(self):
        res = self.get_status_master()
        for i in range(3):
            if res[i][1] == "leader":
                return i
        return None

    def upload_file(self, account, file_name, merkle_sha1, file_size):
        upload_file_request = upload_file_pb2.UploadFileRequest()
        upload_file_request.account     = account
        upload_file_request.file_name   = file_name
        upload_file_request.merkle_sha1 = merkle_sha1
        upload_file_request.file_size   = file_size
        postdata = self.protobuf_to_postdata(upload_file_request)
        (httpcode, response) = self.do_request_post("Master", self.get_master_leader(), "UploadFile", postdata)
        if httpcode == 200:
            upload_file_response = upload_file_pb2.UploadFileResponse()
            upload_file_response.ParseFromString(base64.b64decode(response))
            return (httpcode, upload_file_response)
        else:
            return (httpcode, response)

    def get_oplog(self, index, op_id = None):
        if op_id == None:
            op_id = LexicographicallyGetZero()
        postdata = base64.b64encode("".join(op_id))
        (httpcode, response) = self.do_request_post("Master", index, "GetOplog", postdata)
        if httpcode == 200:
            get_oplog_response = get_oplog_pb2.GetOplogResponse()
            get_oplog_response.ParseFromString(base64.b64decode(response))
            return (httpcode, get_oplog_response)
        else:
            return (httpcode, response)

    def set_file_keyinfo_pb(self, index, set_file_key):
        postdata = self.protobuf_to_postdata(set_file_key)
        return self.do_request_post("Chunk", index, "SetFileKeyinfo", postdata)

    def set_file_keyinfo(self, index, account, file_name, file_hosts, file_size, merkle_sha1, key_aes):
        set_file_key = upload_chunk_file_pb2.SetFileKeyInfo()
        set_file_key.account        = account
        set_file_key.file_name      = file_name
        set_file_key.file_hosts.extend(file_hosts)
        set_file_key.file_size      = file_size
        set_file_key.merkle_sha1    = merkle_sha1
        set_file_key.key_aes        = key_aes

        return self.set_file_keyinfo_pb(index, set_file_key)

    def get_file_keyinfo_pb(self, index, get_file_key):
        postdata = self.protobuf_to_postdata(get_file_key)
        return self.do_request_post("Chunk", index, "GetFileKeyinfo", postdata)

    def get_file_keyinfo(self, index, account, file_name):
        get_file_key = upload_chunk_file_pb2.GetFileKeyInfo()
        get_file_key.account    = account
        get_file_key.file_name  = file_name

        (httpcode, response) = self.get_file_keyinfo_pb(index, get_file_key)
        if httpcode == 200:
            get_file_keyinfo_response = upload_chunk_file_pb2.SetFileKeyInfo()
            get_file_keyinfo_response.ParseFromString(base64.b64decode(response))
            return (httpcode, get_file_keyinfo_response)
        else:
            return (httpcode, response)

    def upload_chunk_file_pb(self, index, proto):
        postdata = self.protobuf_to_postdata(proto)
        return self.do_request_post("Chunk", index, "UploadChunkFile", postdata)

    def upload_chunk_file(self, index, account, file_name, offset, chunkdata):
        upload_chunk_file_request = upload_chunk_file_pb2.UploadChunkFileRequest()
        upload_chunk_file_request.account   = account
        upload_chunk_file_request.file_name = file_name
        upload_chunk_file_request.offset    = offset
        upload_chunk_file_request.chunk     = base64.b64encode(chunkdata)

        return self.upload_chunk_file_pb(index, upload_chunk_file_request)

    def download_chunk_file_pb(self, index, proto):
        postdata = self.protobuf_to_postdata(proto)
        return self.do_request_post("Chunk", index, "DownloadChunkFile", postdata)

    def download_chunk_file(self, index, account, file_name, offset):
        download_chunk_file_request = upload_chunk_file_pb2.DownloadChunkFileRequest()
        download_chunk_file_request.account     = account
        download_chunk_file_request.file_name   = file_name
        download_chunk_file_request.offset      = offset

        return self.download_chunk_file_pb(index, download_chunk_file_request)

if __name__ == "__main__":
    tmst = KyrinboxSdk("test_sdk_config.json", "KeyCenterAdminPublicKey", "../../ci/secret/key.pem")
    fp = open("../../ci/secret/cert.pem", "r")
    print tmst.set_kyrin_key("plusplus7", fp.read())
    fp.close()
    print tmst.get_kyrin_key("plusplus7")
    ts = KyrinboxSdk("test_sdk_config.json", "plusplus7", "../../ci/secret/key.pem")
    print "get_status_master: ", ts.get_status_master()
    print "get_master_leader: ", ts.get_master_leader()
    filename = "sssogs"+str(int(time.time()))
    (httpcode, res) = ts.upload_file("plusplus7", filename, "sha1", 1024)
    print (httpcode, res)
    for i in res.file_hosts:
        index = ts.get_index_by_kbid(int(i.split(" ")[1]))
        print "set_file_keyinfo: ", ts.set_file_keyinfo(index, "plusplus7", filename, res.file_hosts, res.file_size, res.merkle_sha1, "key_aes")
        print "get_file_keyinfo: ", ts.get_file_keyinfo(index, "plusplus7", filename)
        print ts.upload_chunk_file(index, "plusplus7", filename, 0, "chunkdata")
        (httpcode, chunkdata) = ts.download_chunk_file(index, "plusplus7", filename, 0)
        print base64.b64decode(chunkdata)
    print ts.get_oplog(0)
    print ts.get_oplog(1)
    print ts.get_oplog(2)
    (httpcode, res) = ts.get_status_chunk(0)

    print "Seeds: ",
    for i in res.seeds:
        print i.kbid,
    print ""
    print "Commons: ",
    for i in res.commons:
        print i.kbid,
    print ""
    print "Dead seeds: ",
    for i in res.dead_seeds:
        print i.kbid
    print ""
    print "Dead commons: ",
    for i in res.dead_commons:
        print i.kbid
    print ""
    for i in res.datas:
        print "Datas: ",
        print i.kbid,
        print i.host,
        print i.gossip_port
