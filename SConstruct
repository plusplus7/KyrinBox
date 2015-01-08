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

env = Environment(CC = 'gcc', CCFLAGS = '-O2')
#dbg = Environment(CC = 'gcc', CCFLAGS = '-g')

if os.path.exists("src/external/") == False:
    prepare_for_external()

os.system("./src/external/protobuf-2.6.0/src/protoc -I=./src/protobuf --cpp_out=./src/protobuf ./src/protobuf/test.proto")

### Append paths
env.Append(CPPPATH = ['src'])
env.Append(CPPPATH = ['src/protobuf'])
env.Append(CPPPATH = ['src/external/include'])
env.Append(LIBPATH = ['.'])
env.Append(LIBPATH = ['src/external/lib'])

### Compile statics
env.StaticLibrary(target = 'kyrin_base_server', source = 'src/server/base/kyrin_base_server.cc')
env.StaticLibrary(target = 'kyrin_constants', source = 'src/common/kyrin_constants.cc')
env.StaticLibrary(target = 'kyrin_database_wrapper', source = 'src/io/kyrin_database_wrapper.cc')
env.StaticLibrary(target = 'kyrin_log', source = 'src/common/kyrin_log.cc')
env.StaticLibrary(target = 'kyrin_master_server', source = 'src/server/master/kyrin_master_server.cc')
env.StaticLibrary(target = 'kyrin_config', source = 'src/common/kyrin_config.cc')
env.StaticLibrary(target = 'proto_test', source = 'src/protobuf/test.pb.cc')

### Link
kyrin_master = env.Program('kyrin_master', 'src/server/master/kyrin_master_main.cc', LIBS = ['kyrin_master_server', 'event', 'kyrin_base_server', 'kyrin_log', 'kyrin_database_wrapper', 'leveldb', 'kyrin_constants', 'kyrin_config'], )

test_protobuf = env.Program("test_protobuf", 'src/test/test_protobuf.cc', LIBS = ['proto_test', 'protobuf'])
test_spinlock = env.Program('test_spinlock', 'src/test/test_spinlock.cpp', LIBS = ['pthread', ])

env.Install('release/bin', kyrin_master)
env.Install('release/test', 'src/test/run_test.sh')
