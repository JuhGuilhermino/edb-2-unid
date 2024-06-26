cmake_minimum_required(VERSION 3.5)
project (Fauna VERSION 1.0.0 LANGUAGES CXX )

# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== SETTING VARIABLES ===#
# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall" )
#set( PREPROCESSING_FLAGS  "-D PRINT -D DEBUG -D CASE="WORST" -D ALGO="QUAD"')
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${PREPROCESSING_FLAGS}" )

#=== Main App ===

#Include dir
include_directories( lib )
add_executable(fauna
    "lib/btree.h"
    "lib/rbtree.h"
    "lib/avl-tree.h"
    "common.h"
    "main.cpp"
    "animal.cpp" 
    "history.cpp" 
    "program.cpp"
    )

target_compile_features( fauna PUBLIC cxx_std_17 )
