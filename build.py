#!python

import os
import subprocess
import sys

clang_compiler = r'C:\Program Files\LLVM\bin\clang++.exe'
if not os.path.exists(clang_compiler):
    print("Build failed: Could not find %s" % (clang_compiler))
    sys.exit(-1)
build_command = "\"%s\" -fcolor-diagnostics -fansi-escape-codes -g example.cpp -o example.exe" % (
    clang_compiler)
process = subprocess.Popen(build_command, shell=True)
process.wait()
sys.exit(process.returncode)
