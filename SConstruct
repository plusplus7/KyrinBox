import os

env = Environment(CC = 'gcc', CCFLAGS = '-O2')
#dbg = Environment(CC = 'gcc', CCFLAGS = '-g')

env.Append(CPPPATH = ['src'])
env.Append(CPPPATH = ['src/external/include'])
env.Append(LIBPATH = ['.'])
env.Append(LIBPATH = ['src/external/lib'])

env.StaticLibrary(target = 'kyrin_base_server', source = 'src/server/kyrin_base_server.cc')
env.StaticLibrary(target = 'kyrin_database_wrapper', source = 'src/io/kyrin_database_wrapper.cc')
env.StaticLibrary(target = 'kyrin_log', source = 'src/common/kyrin_log.cc')
env.StaticLibrary(target = 'kyrin_master_server', source = 'src/server/master/kyrin_master_server.cc')

env.Program('kyrin_master', 'src/server/master/kyrin_master_main.cc', LIBS = ['kyrin_master_server', 'event', 'kyrin_base_server', 'kyrin_log', 'kyrin_database_wrapper', 'leveldb',], )
