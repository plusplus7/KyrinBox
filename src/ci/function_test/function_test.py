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

    def test_GetOplog_Ten(self):
        filenames = []
        for i in range(10):
            filename = "sssogs" + str(int(time.time()*1000) + 1)
            ret = self.sdk.upload_file("plusplus7", filename, "sha1", 1024)
            filenames.append(filename)
            self.assertEqual(ret[0], 200)
        ret = self.sdk.get_oplog(0)
        self.assertEqual(ret[0], 200)
        op = operation_log_pb2.OperationLog()
        for i in range(10):
            op.ParseFromString(ret[1].log_data[-(i+1)])
            ufo = upload_file_pb2.UploadFileOperation()
            ufo.ParseFromString(op.log_data)
            self.assertEqual(ufo.key, "plusplus7"+filenames[9-i])
            self.assertEqual(ufo.data.file_size, 1024)
            self.assertEqual(ufo.data.merkle_sha1, "sha1")

if __name__ == "__main__":
    unittest.main()
