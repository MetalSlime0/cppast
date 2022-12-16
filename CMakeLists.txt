# Copyright (C) 2017-2022 Jonathan Müller and cppast contributors
# SPDX-License-Identifier: MIT
# found in the top-level directory of this distribution.

cmake_minimum_required(VERSION 3.11)
project(cppast VERSION 0.1.0)

# options
option(CPPAST_BUILD_TEST "whether or not to build the tests" OFF)
option(CPPAST_BUILD_EXAMPLE "whether or not to build the examples" OFF)
option(CPPAST_BUILD_TOOL "whether or not to build the tool" OFF)
set(LLVM_CONFIG_BINARY "../llvm/llvm-project-llvmorg-14.0.0/llvm/Release/bin/llvm-config.exe")
set(LIBCLANG_INCLUDE_DIR "../llvm/llvm-project-llvmorg-14.0.0/clang/include")
set(LIBCLANG_LIBRARY "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/Llvm/x64/lib/libclang.lib")
set(CLANG_BINARY "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/Llvm/x64/bin/clang.exe")

if(${CPPAST_BUILD_TEST} OR (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    set(build_test ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for the self integration test
else()
    set(build_test OFF)
endif()

if(${CPPAST_BUILD_EXAMPLE} OR (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    set(build_example ON)
else()
    set(build_example OFF)
endif()

if(${CPPAST_BUILD_TOOL} OR (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    set(build_tool ON)
else()
    set(build_tool OFF)
endif()

include(external/external.cmake)

if(build_test AND CPPAST_TEST_GCOV AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
endif()

add_subdirectory(src)

if(${build_test})
    enable_testing()
    add_subdirectory(test)
endif()
if(${build_example})
    add_subdirectory(example)
endif()
if(${build_tool})
    add_subdirectory(tool)
endif()
