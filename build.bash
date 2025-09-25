#!/bin/bash

# Compiler
CXX=g++

# Compile flags
CXXFLAGS="-std=c++11 -Wall -Iinclude"

# Raylib and system libraries (for Linux)
LIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

# Output file
OUT="main"

# Find all source files in src/
SRC=$(find src -name "*.cpp")

# Build command
$CXX $CXXFLAGS main.cpp $SRC $LIBS -o $OUT
