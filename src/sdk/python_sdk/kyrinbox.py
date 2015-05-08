#! /usr/bin/env python
import os
import sys
import time
import base64
import hashlib
import optparse
import kyrinbox_sdk
import M2Crypto

def get_parser():
    usage = "usage: %prog -n arg0 -k arg1 -u/d arg2"
    parser = optparse.OptionParser(usage=usage)
    parser.add_option("-n", type="string", dest="username", help="Assign username to operate", default="test")
    parser.add_option("-k", type="string", dest="keypath", help="Assign path to the private key", default="/etc/kyrinbox/.key/key.pem")
    parser.add_option("-u", type="string", dest="upload_filename", help="Assign the filename to upload", default="")
    parser.add_option("-d", type="string", dest="download_filename", help="Assign the filename to download", default="")
    return parser

def get_cryptor(op, key, alg = 'aes_128_cfb', iv = None):
    if iv == None:
        iv = '\0'*16
    return M2Crypto.EVP.Cipher(alg=alg, key=key, iv=iv, op=op)

def upload_file(host, set_file_keyinfo_port, upload_chunk_port, username, filename):
    fp = open(filename, "rb+")
    key = os.urandom(32)
    (httpcode, res) = sdk.set_file_keyinfo_invoke(host, set_file_keyinfo_port, username, filename, [], os.stat(filename).st_size, "",  base64.b64encode(key))
    if httpcode != 200:
        return "Failed: ", res
    idx = 0
    while True:
        pr = fp.tell()
        tmp = fp.read(1024)
        cryptor = get_cryptor(1, key)
        tmp = cryptor.update(tmp)
        tmp = tmp + cryptor.final()
        if pr == fp.tell():
            break
        (httpcode, res) = sdk.upload_chunk_file_invoke(host, upload_chunk_port, username, filename, idx*1024, tmp)
        if httpcode != 200:
            return "Failed: ", res
        idx = idx+1
    fp.close()
    return "Success"

def download_file(host, get_file_keyinfo_port, download_chunk_port, username, filename):
    os.system("touch " + filename + ".down")
    fp = open(filename+".down", "rb+")
    idx = 0
    (httpcode, res) = sdk.get_file_keyinfo_invoke(host, get_file_keyinfo_port, username, filename)
    if httpcode != 200:
        return "Failed: ", res 
    keyinfo = res 
    key = base64.b64decode(keyinfo.key_aes)
    while True:
        (httpcode, res) = sdk.download_chunk_file_invoke(host, download_chunk_port, username, filename, idx*1024)
        if httpcode != 200:
            return "Failed: ", res
        idx = idx + 1
        if len(res) == 0:
            break
        tmp = base64.b64decode(res)
        cryptor = get_cryptor(0 , key)
        ret = cryptor.update(tmp)
        ret = ret + cryptor.final()
        fp.write(ret)
    fp.close()

    return "Success"

if __name__ == "__main__":
    parser = get_parser()
    (options, args) = parser.parse_args()
    sdk = kyrinbox_sdk.KyrinboxSdk("/etc/kyrinbox/kyrinbox_server.json", options.username, options.keypath)
    if options.upload_filename != "":

        (httpcode, res) = sdk.upload_file(options.username, options.upload_filename, "", os.stat(options.upload_filename).st_size)
        print "upload file: ", (httpcode, res)
        time.sleep(1.5)
        (httpcode, res) = sdk.download_file(options.username, options.upload_filename)
        (httpcode, chunkstatus) = sdk.get_status_chunk(0)
        kbmap = {}
        for data in chunkstatus.datas:
            kbmap[str(data.kbid)] = data
        for i in res["chunks"]:
            host  = kbmap[i["kbid"]].host
            sport = kbmap[i["kbid"]].set_file_keyinfo_port
            uport = kbmap[i["kbid"]].upload_chunk_port
            print "upload: ", upload_file(host, sport, uport, options.username, options.upload_filename)

    if options.download_filename != "":

        (httpcode, res) = sdk.download_file(options.username, options.download_filename)
        print "download file: ", (httpcode, res)
        (httpcode, chunkstatus) = sdk.get_status_chunk(0)
        kbmap = {}
        for data in chunkstatus.datas:
            kbmap[str(data.kbid)] = data
        for i in res["chunks"]:
            host = kbmap[i["kbid"]].host
            sfkp = kbmap[i["kbid"]].get_file_keyinfo_port
            ucp  = kbmap[i["kbid"]].download_chunk_port
            print "download: ", download_file(host, sfkp, ucp, options.username, options.download_filename)
