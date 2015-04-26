#coding=utf-8
import sys

fp = open(sys.argv[1], "r")
str = fp.read()
fp.close()

fp = open(sys.argv[2], "w")
fp.write(str.encode("hex"))
fp.close()

