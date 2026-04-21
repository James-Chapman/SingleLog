# Copyright(c) 2016-2024, James Chapman
#
# Use of this source code is governed by a BSD -
# style license that can be found in the LICENSE file or
# at https://choosealicense.com/licenses/bsd-3-clause/

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
    # Source and header directories
    src_dir = "."
    include_dir = "."

    # Define build directories based on OS and build type
    output_dir = "build"
    build_dir = os.path.join(output_dir, build_type)
    obj_dir = os.path.join(build_dir, "obj")
    compiler = "g++"
    flags = []
    std_flag = "-std=c++20"
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

    if platform.system() == "Windows":
        compiler = "cl"
        std_flag = "/std:c++20"
        additional_flags = ["/W4", "/WX"]  # Equivalent to -Wall and -pedantic

    # Define flags based on build type
    if build_type == "release":
        if platform.system() == "Windows":
            flags = ["/O2", "/EHsc", std_flag] + additional_flags
        else:
            flags = ["-O3", "-DNDEBUG", std_flag] + additional_flags
    else:
        if platform.system() == "Windows":
            flags = ["/Zi", "/Od", "/EHsc", std_flag] + additional_flags
        else:
            flags = ["-g", "-O0", "-Wall", "-pedantic", std_flag] + additional_flags

    # Create build directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)
    os.makedirs(build_dir, exist_ok=True)
    os.makedirs(obj_dir, exist_ok=True)

    # Get all source files from the src directory
    source_files = [
        os.path.join(src_dir, f) for f in os.listdir(src_dir) if f.endswith(".cpp")
    ]

    # Compile each source file with appropriate flags
    object_files = []
    for source_file in source_files:
        object_file = os.path.join(
            obj_dir, os.path.splitext(os.path.basename(source_file))[0] + ".o"
        )
        command = [
            compiler,
            "-c",
            f"-I{include_dir}",
            "-o",
            object_file,
            source_file,
        ] + flags
        print(f"Compiling: {source_file} -> {object_file}")
        subprocess.run(command, check=True)
        object_files.append(object_file)

    # Link object files into the final executable
    output_file = os.path.join(build_dir, "SingleLogExample")
    link_command = (
        [compiler] + linker_flags + object_files + [f"-L{obj_dir}", "-o", output_file]
    )
    print(f"Linking: {object_files} -> {output_file}")
    subprocess.run(link_command, check=True)

    print(f"Build completed successfully! Output: {output_file}")


if __name__ == "__main__":
    # Get build type from command line argument (optional)
    build_type = "release"
    if len(sys.argv) > 1:
        build_type = sys.argv[1].lower()
    if build_type not in ("release", "debug"):
        print(
            f"Invalid build type '{build_type}'. Valid options are 'release' or 'debug'."
        )
        sys.exit(1)

    build_project(build_type)
