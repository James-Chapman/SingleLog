# Copyright(c) 2016-2026, James Chapman
#
# Use of this source code is governed by a BSD -
# style license that can be found in the LICENSE file or
# at https://choosealicense.com/licenses/bsd-3-clause/
"""Build script for the SingleLog example. Usage: python build.py [release|debug]"""

import os
import platform
import sys
import subprocess


def build_project(build_type):
    """
    Builds the CPP project with the specified build type (release or debug).

    Args:
      build_type: String representing the build type ("release" or "debug").
    """
    src_dir = "."
    include_dir = "."

    output_dir = "build"
    build_dir = os.path.join(output_dir, build_type)
    obj_dir = os.path.join(build_dir, "obj")

    is_windows = platform.system() == "Windows"

    if is_windows:
        compiler = "cl"
        std_flag = "/std:c++20"
        obj_ext = ".obj"
        linker_flags = []
        if build_type == "release":
            flags = ["/O2", "/EHsc", std_flag, "/W4", "/WX"]
        else:
            flags = ["/Zi", "/Od", "/EHsc", std_flag, "/W4", "/WX"]
    else:
        compiler = "g++"
        std_flag = "-std=c++20"
        obj_ext = ".o"
        additional_flags = [
            "-Wall",
            "-pedantic",
            "-Weffc++",
            "-Wextra",
            "-Werror",
            "-Wconversion",
            "-Wsign-conversion",
            "-Wshadow",
            "-Wold-style-cast",
            "-Woverloaded-virtual",
            "-Wfloat-equal",
            "-Wwrite-strings",
            "-Wpointer-arith",
            "-Wcast-qual",
            "-Wcast-align",
            "-Wswitch-enum",
            "-Wswitch-default",
            "-Winit-self",
            "-Wredundant-decls",
            "-Wlogical-op",
            "-Winline",
            "-Wunreachable-code",
            "-Wmissing-declarations",
            "-Wno-unused",
            "-Wfatal-errors",
            "-fdiagnostics-show-option",
            "-fsanitize=address",
        ]
        linker_flags = ["-fsanitize=address"]
        if build_type == "release":
            flags = ["-O3", "-DNDEBUG", std_flag] + additional_flags
        else:
            flags = ["-g", "-O0", std_flag] + additional_flags

    os.makedirs(output_dir, exist_ok=True)
    os.makedirs(build_dir, exist_ok=True)
    os.makedirs(obj_dir, exist_ok=True)

    source_files = [
        os.path.join(src_dir, f) for f in os.listdir(src_dir) if f.endswith(".cpp")
    ]

    object_files = []
    for source_file in source_files:
        base = os.path.splitext(os.path.basename(source_file))[0]
        object_file = os.path.join(obj_dir, base + obj_ext)
        if is_windows:
            # MSVC: /c compiles only, /Fo sets object output, /I sets include path
            command = [compiler, "/c"] + flags + [f"/I{include_dir}", f"/Fo{object_file}", source_file]
        else:
            command = [compiler, "-c", f"-I{include_dir}", "-o", object_file, source_file] + flags
        print(f"Compiling: {source_file} -> {object_file}")
        subprocess.run(command, check=True)
        object_files.append(object_file)

    output_file = os.path.join(build_dir, "SingleLogExample")
    if is_windows:
        output_file += ".exe"
        # MSVC: /Fe sets executable output path
        link_command = [compiler] + object_files + [f"/Fe{output_file}"] + linker_flags
    else:
        link_command = [compiler] + linker_flags + object_files + [f"-L{obj_dir}", "-o", output_file]

    print(f"Linking: {object_files} -> {output_file}")
    subprocess.run(link_command, check=True)

    print(f"Build completed successfully! Output: {output_file}")


if __name__ == "__main__":
    BUILD_TYPE = sys.argv[1].lower() if len(sys.argv) > 1 else "release"
    if BUILD_TYPE not in ("release", "debug"):
        print(f"Invalid build type '{BUILD_TYPE}'. Valid options are 'release' or 'debug'.")
        sys.exit(1)

    build_project(BUILD_TYPE)
