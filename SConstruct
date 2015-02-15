import os

def prepare_for_external():
    os.system("mkdir src/external && mkdir src/external/include && mkdir src/external/lib")

    ### Prepare for leveldb
    os.system("rm -rf leveldb-master/")
    os.system("unzip miscs/softwares/leveldb-master.zip")
    os.system("cd leveldb-master && make -j4")
    os.system("cp leveldb-master/libleveldb.a src/external/lib")
    os.system("cp -r leveldb-master/include/leveldb src/external/include")
    os.system("rm -rf leveldb-master/")

    ### Prepare for protobuf
    os.system("tar zxvf miscs/softwares/protobuf-2.6.0.tar.gz -C src/external/")
    os.system("cd src/external/protobuf-2.6.0 && ./configure && make -j4")
    os.system("cp -r src/external/protobuf-2.6.0/src/google src/external/include")
    os.system("cp src/external/protobuf-2.6.0/src/*.la src/external/lib")

    ### Prepare for openssl
    os.system("tar zxvf miscs/softwares/openssl-1.0.2.tar.gz -C src/external/")
    os.system("cd src/external/openssl-1.0.2 && ./Configure darwin64-x86_64-cc && make")
    os.system("cp -r src/external/openssl-1.0.2/include/openssl src/external/include")
    os.system("cp src/external/openssl-1.0.2/*.a src/external/lib")
    os.system("rm -rf src/external/openssl-1.0.2/")

def compile_protobuf(proto_list):
    for i in proto_list:
        os.system("./src/external/protobuf-2.6.0/src/protoc -I=./src/protobuf --cpp_out=./src/protobuf --python_out=./src/protobuf ./src/protobuf/" + i)

env = Environment(CC = 'gcc', CCFLAGS = '-O2')
#dbg = Environment(CC = 'gcc', CCFLAGS = '-g')

if os.path.exists("src/external/") == False:
    prepare_for_external()

proto_list = ["test.proto",
              "upload_file.proto",
              "operation_log.proto",
              "get_oplog.proto",
             ]
compile_protobuf(proto_list)

### Append paths
env.Append(CPPPATH = ['src'])
env.Append(CPPPATH = ['src/protobuf'])
env.Append(CPPPATH = ['src/external/include'])
env.Append(LIBPATH = ['.'])
env.Append(LIBPATH = ['src/external/lib'])

### Compile statics
env.StaticLibrary(target = 'kyrin_base_server', source = 'src/server/base/kyrin_base_server.cc')
env.StaticLibrary(target = 'kyrin_constants', source = 'src/common/kyrin_constants.cc')
env.StaticLibrary(target = 'kyrin_base64', source = 'src/common/crypto/kyrin_base64.cc')
env.StaticLibrary(target = 'kyrin_sha1', source = 'src/common/crypto/kyrin_sha1.cc')
env.StaticLibrary(target = 'kyrin_database_wrapper', source = 'src/io/kyrin_database_wrapper.cc')
env.StaticLibrary(target = 'kyrin_http_client', source = 'src/io/kyrin_http_client.cc')
env.StaticLibrary(target = 'kyrin_log', source = 'src/common/kyrin_log.cc')
env.StaticLibrary(target = 'upload_file_request_handler', source = 'src/server/request_handlers/upload_file_request_handler.cc')
env.StaticLibrary(target = 'get_oplog_request_handler', source = 'src/server/request_handlers/get_oplog_request_handler.cc')
env.StaticLibrary(target = 'confirm_oplog_request_handler', source = 'src/server/request_handlers/confirm_oplog_request_handler.cc')
env.StaticLibrary(target = 'kyrin_master_server', source = 'src/server/master/kyrin_master_server.cc')
env.StaticLibrary(target = 'kyrin_master_sentinel', source = 'src/server/master/kyrin_master_sentinel.cc')
env.StaticLibrary(target = 'kyrin_master_sentinel_server', source = 'src/server/master/kyrin_master_sentinel_server.cc')
env.StaticLibrary(target = 'kyrin_base_config', source = 'src/common/configs/kyrin_base_config.cc')
env.StaticLibrary(target = 'kyrin_cluster', source = 'src/common/kyrin_cluster.cc')
env.StaticLibrary(target = 'proto_test', source = 'src/protobuf/test.pb.cc')
env.StaticLibrary(target = 'proto_upload_file', source = 'src/protobuf/upload_file.pb.cc')
env.StaticLibrary(target = 'proto_operation_log', source = 'src/protobuf/operation_log.pb.cc')
env.StaticLibrary(target = 'proto_get_oplog', source = 'src/protobuf/get_oplog.pb.cc')

### Link
kyrin_master = env.Program('kyrin_master', 'src/server/master/kyrin_master_main.cc',
                            LIBS = ['kyrin_master_server',
                                    'event',
                                    'kyrin_base_server',
                                    'kyrin_log',
                                    'get_oplog_request_handler',
                                    'confirm_oplog_request_handler',
                                    'upload_file_request_handler',
                                    'kyrin_database_wrapper',
                                    'leveldb',
                                    'kyrin_constants',
                                    'kyrin_base_config',
                                    'kyrin_cluster',
                                    'kyrin_master_sentinel',
                                    'kyrin_master_sentinel_server',
                                    'curl',
                                    'kyrin_http_client',
                                    'kyrin_base64',
                                    'proto_upload_file',
                                    'proto_operation_log',
                                    'proto_get_oplog',
                                    'protobuf',
                                   ],
                          )

test_protobuf = env.Program("test_protobuf", 'src/test/test_protobuf.cc', LIBS = ['proto_test', 'protobuf'])
test_spinlock = env.Program('test_spinlock', 'src/test/test_spinlock.cpp', LIBS = ['pthread', ])
test_http_client= env.Program('test_http_client', 'src/test/test_http_client.cpp', LIBS = ['event', 'curl', 'kyrin_http_client'])
test_lexicographically_helper = env.Program('test_lexicographically_helper', 'src/test/test_lexicographically_helper.cpp', LIBS = ['leveldb', ])
test_sha1 = env.Program('test_sha1', 'src/test/test_sha1.cpp', LIBS = ['kyrin_sha1', 'ssl', 'crypto'])

### release
env.Install('release/bin', kyrin_master)
