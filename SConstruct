# -*- mode: Python -*-

import os
import glob

env = Environment(ENV = {'PATH' : os.environ['PATH']})
SOURCES = glob.glob('src/c/*.cpp')
HEADERS = glob.glob('src/headers/*.h')

env.Append(CCFLAGS = ['-g', '-Wall', '-std=c++11', '-Isrc/headers/'])

env.Program(target = 'bin/main', source = SOURCES)
