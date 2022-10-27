#!python

import argparse
import os
import string
import subprocess
import sys

g_bin_name = "example.exe"
g_clang_compiler = r'C:\Program Files\LLVM\bin\clang++.exe'
g_source_files = ['example.cpp']
g_preproc_defs = []


def checkFilePaths():
    if not os.path.exists(g_clang_compiler):
        print("Build failed: Could not find %s" % (g_clang_compiler))
        sys.exit(-1)

    # Check source files exist
    for source_file in g_source_files:
        if not os.path.exists(source_file):
            print("Build failed: Could not find %s" % (source_file))
            sys.exit(-1)


def build(args) -> int:
    preproc_string = ""
    if len(g_preproc_defs) > 0:
        preproc_string = "-D" + " ".join(g_preproc_defs)

    source_files_string = ""
    if len(g_source_files) > 0:
        source_files_string = " ".join(g_source_files)

    pdb_string = ""
    if args.pdb:
        pdb_string = "-g "

    os.makedirs('build', exist_ok=True)

    # Build
    build_command = "\"%s\" -fcolor-diagnostics -fansi-escape-codes %s %s %s -o build/%s" % (
        g_clang_compiler, pdb_string, source_files_string, preproc_string, g_bin_name)
    print(build_command)
    process = subprocess.Popen(build_command, shell=True)
    process.wait()
    return process.returncode


def main() -> int:
    """Echo the input arguments to standard output"""
    parser = argparse.ArgumentParser(prog='build.py',)
    parser.add_argument('--pdb', action='store_true')  # on/off flag
    args = parser.parse_args()
    checkFilePaths()
    return build(args)


if __name__ == '__main__':
    sys.exit(main())  # next section explains the use of sys.exit
