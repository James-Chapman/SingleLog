import os
import sys

# Define the build configurations
env = Environment()

if sys.platform == "win32":
    env["CC"] = "cl"
    env["CXX"] = "cl"
elif sys.platform == "linux":
    env["CC"] = "gcc"
    env["CXX"] = "g++"
elif sys.platform == "darwin":
    env["CC"] = "clang"
    env["CXX"] = "clang++"
else:
    raise Exception("Unsupported platform")

# Source files
sources = ['example.cpp']

# Release configuration
release_env = env.Clone()
if sys.platform == "win32":
    release_env.Append(CCFLAGS=['/std:c++14', '/EHsc', '/O2', '/MT'])
    release_env.Append(CPPDEFINES=["WIN32"])
elif sys.platform == "linux":
    release_env.Append(CCFLAGS=['-std=c++14', '-O2'])
    release_env.Append(LINKFLAGS=['-lstdc++'])
elif sys.platform == "darwin":
    release_env.Append(CCFLAGS=['-std=c++14', '-O2'])
    release_env.Append(LINKFLAGS=['-stdlib=libc++'])

release_env.Append(CPPDEFINES=["NDEBUG"])
release_env.VariantDir(Dir("release_obj"), ".", duplicate=0)
release_env.Install(Dir("release"), 'SingleLogExample')

# Debug configuration
debug_env = env.Clone()
if sys.platform == "win32":
    debug_env.Append(CCFLAGS=['/fsanitize=address', '/Od', '/MTd'])
    debug_env.Append(LINKFLAGS=['/fsanitize=address'])
elif sys.platform == "linux":
    debug_env.Append(CCFLAGS=['-g', '-O0', '-fsanitize=address'])
    debug_env.Append(LINKFLAGS=['-g', '-fsanitize=address'])
elif sys.platform == "darwin":
    debug_env.Append(CCFLAGS=['-g', '-O0', '-fsanitize=address'])
    debug_env.Append(LINKFLAGS=['-g', '-fsanitize=address'])

debug_env.Append(CPPDEFINES=["DEBUG"])
debug_env.VariantDir(Dir("debug_obj"), ".", duplicate=0)
debug_env.Install(Dir("debug"), 'SingleLogExample')

target = release_env.Program('SingleLogExample', sources)
# target = debug_env.Program('SingleLogExample', sources)

# Define the build targets
Default(target)
