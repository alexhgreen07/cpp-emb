
env = Environment(CPPFLAGS='-std=c++11',CPPPATH=['emb','cpputest/include']) 

cpputest_sources = Glob("cpputest/src/CppUTest/*.cpp")
cpputest_platform = Glob("cpputest/src/Platforms/Gcc/*.cpp")

lib_sources = Glob("emb/*.cpp")
test_sources = Glob("test/*.cpp")

sources = test_sources + lib_sources + cpputest_sources + cpputest_platform
env.Program(target = 'bin/test.exe', source = sources)