#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import time
import unittest
import kyrinbox_sdk
import upload_file_pb2
import get_oplog_pb2
import operation_log_pb2

class FindLeaderTest(unittest.TestCase):

    def setUp(self):
        self.sdk = kyrinbox_sdk.KyrinboxSdk("./test_sdk_config.json", "plusplus7", "../secret/key.pem")
        pass

    def tearDown(self):
        pass

    def test_get_leader(self):
        ret = self.sdk.get_master_leader()
        self.assertNotEqual(ret, None)

class UploadFileTest(unittest.TestCase):

    def setUp(self):
        self.sdk = kyrinbox_sdk.KyrinboxSdk("./test_sdk_config.json", "plusplus7", "../secret/key.pem")
        pass

    def tearDown(self):
        pass

    def test_upload_one(self):
        filename = "sssogs" + str(int(time.time()*1000))
        ret = self.sdk.upload_file("plusplus7", filename, "sha1", 1024)
        self.assertEqual(ret[0], 200)

class GetOplogTest(unittest.TestCase):

    def setUp(self):
        self.sdk = kyrinbox_sdk.KyrinboxSdk("./test_sdk_config.json", "plusplus7", "../secret/key.pem")
        pass

    def tearDown(self):
        pass

    def test_GetOplog_None(self):
        ret = self.sdk.get_oplog(0)
        self.assertEqual(ret[0], 400)

    def test_GetOplog_One(self):
        filename = "sssogs" + str(int(time.time()*1000))
        ret = self.sdk.upload_file("plusplus7", filename, "sha1", 1024)
        self.assertEqual(ret[0], 200)
        ret = self.sdk.get_oplog(0)
        self.assertEqual(ret[0], 200)
        op = operation_log_pb2.OperationLog()
        op.ParseFromString(ret[1].log_data[-1])
        ufo = upload_file_pb2.UploadFileOperation()
        ufo.ParseFromString(op.log_data)
        self.assertEqual(ufo.data.file_size, 1024)
        self.assertEqual(ufo.data.merkle_sha1, "sha1")

"""
    def test_GetOplog_Ten(self):
        for i in range(10):
            ret = test_sdk.UploadFile(self.host, self.upload_file_port, self.get_new_request(), self.headers)
            self.assertEqual(ret[0], 200)
        ret = test_sdk.GetOplog(self.host, self.get_oplog_port)
        self.assertEqual(ret[0], 200)
        op = operation_log_pb2.OperationLog()
        for i in range(1, 11):
            op.ParseFromString(ret[1].log_data[-i])
            ufo = upload_file_pb2.UploadFileOperation()
            ufo.ParseFromString(op.log_data)
            self.assertEqual(ufo.key, self.get_prev_key(i))
            self.assertEqual(ufo.data.file_size, 1024)
            self.assertEqual(ufo.data.merkle_sha1, "sha1")
class TwoPhaseTest(unittest.TestCase):

    seqnum = 0
    def setUp(self):
        self.host = "localhost"
        self.sentinel_ports = (7777, 17777, 27777)
        ret = test_sdk.FindLeader(self.host, self.sentinel_ports)
        self.assertNotEqual(ret, None)
        self.get_oplog_ports = (17070, 27070, 37070)
        self.upload_file_port = 7770 + (ret[1]+1)*10000
        self.headers = {'KYRIN-TIMESTAMP' : "19930916",
                   'KYRIN-SIGNATURE' : "signature"}
        pass

    def tearDown(self):
        pass

    def get_new_request(self):
        uf_request = upload_file_pb2.UploadFileRequest()
        uf_request.account = "two_phase_test"
        GetOplogTest.seqnum = GetOplogTest.seqnum+1
        uf_request.file_name = "test%d" % (GetOplogTest.seqnum, )
        uf_request.merkle_sha1 = "sha1"
        uf_request.file_size = 1024
        return uf_request

    def get_prev_key(self, last = 1):
        return "two_phase_test" + "test%d" % (GetOplogTest.seqnum-last+1, )

    def test_UploadFile_and_GetOplog_one(self):
        ret = test_sdk.UploadFile(self.host, self.upload_file_port, self.get_new_request(), self.headers)
        self.assertEqual(ret[0], 200)
        for i in self.get_oplog_ports:
            ret = test_sdk.GetOplog(self.host, i)
            self.assertEqual(ret[0], 200)
            op = operation_log_pb2.OperationLog()
            op.ParseFromString(ret[1].log_data[-1])
            ufo = upload_file_pb2.UploadFileOperation()
            ufo.ParseFromString(op.log_data)
            self.assertEqual(ufo.key, self.get_prev_key())
            self.assertEqual(ufo.data.file_size, 1024)
            self.assertEqual(ufo.data.merkle_sha1, "sha1")

    def test_UploadFile_and_GetOplog_ten(self):
        for i in range(10):
            ret = test_sdk.UploadFile(self.host, self.upload_file_port, self.get_new_request(), self.headers)
            self.assertEqual(ret[0], 200)
        for port in self.get_oplog_ports:
            ret = test_sdk.GetOplog(self.host, port)
            self.assertEqual(ret[0], 200)
            op = operation_log_pb2.OperationLog()
            for i in range(1, 11):
                op.ParseFromString(ret[1].log_data[-i])
                ufo = upload_file_pb2.UploadFileOperation()
                ufo.ParseFromString(op.log_data)
                self.assertEqual(ufo.key, self.get_prev_key(i))
                self.assertEqual(ufo.data.file_size, 1024)
                self.assertEqual(ufo.data.merkle_sha1, "sha1")
"""
if __name__ == "__main__":
    unittest.main()
