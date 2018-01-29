import os
env = Environment(ENV = {'PATH' : os.environ['PATH'],
                         'HOME' : os.environ['HOME']})

env['CXX'] = 'g++'

env.Program('SingleLogExample', 'example.cpp')