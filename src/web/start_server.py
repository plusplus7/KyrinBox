import optparse
import urllib2
import thread
import base64
import time
import json
import sys
import os
import chunk_cluster_status_pb2

def build_files(secret):
    pass

def get_parser():
    usage = "usage: %prog -p arg1 -s arg2 -c arg3"
    parser = optparse.OptionParser(usage = usage)
    parser.add_option("-p", type="int", dest="port", help="Local port of the web system", default = 80)
    parser.add_option("-s", type="string", dest="secret", help="Secret to access the web system", default="asdf")
    parser.add_option("-c", type="string", dest="config", help="Config description of the kyrinbox system", default="../ci/function_test/test_sdk_config.json")
    return parser

def run_status_checker(config):
    fp = open(config, "r")
    config = json.load(fp)
    fp.close()
    while True:
        index = 1
        for m in config["Master"]:
            req = urllib2.Request("http://" + m["Host"] + ":" + str(m["GetStatusPort"]) + "/GetStatus")
            data = ""
            try:
                res = urllib2.urlopen(req)
                data = res.read()
            except:
                data = "down"

            master_fp = open("./html/status/%s" % (("Master" + str(index)), ), "w")
            master_fp.write(data)
            master_fp.close()
            index = index + 1
        index = 1
        for m in config["Chunk"]:
            req = urllib2.Request("http://" + m["Host"] + ":" + str(m["GetStatusPort"]) + "/GetStatus", "Read")
            data = ""
            try:
                jdata = {}
                res = urllib2.urlopen(req)
                chunk_status  = chunk_cluster_status_pb2.ChunkClusterStatus()
                chunk_status.ParseFromString(base64.b64decode(res.read()))
                seeds_list = []
                for i in chunk_status.seeds:
                    seeds_list.append(i.kbid)
                jdata["Seeds"] = seeds_list

                commons_list = []
                for i in chunk_status.commons:
                    commons_list.append(i.kbid)
                jdata["Commons"] = commons_list 

                dead_seeds_list = []
                for i in chunk_status.dead_seeds:
                    dead_seeds_list.append(i.kbid)
                jdata["DeadSeeds"] = dead_seeds_list

                dead_commons_list = []
                for i in chunk_status.dead_commons:
                    dead_commons_list.append(i.kbid)
                jdata["DeadCommons"] = dead_commons_list
                data = json.dumps(jdata)

            except:
                data = "down"

            master_fp = open("./html/status/%s" % (("Chunk" + str(index)), ), "w")
            master_fp.write(data)
            master_fp.close()
            index = index + 1
        time.sleep(1)
    
if __name__ == "__main__":
    parser = get_parser()
    (options, args) = parser.parse_args()
    build_files(options.secret)
    thread.start_new_thread(run_status_checker, (options.config, ))
    os.system("cd html && python -m SimpleHTTPServer %d" % (options.port,))
