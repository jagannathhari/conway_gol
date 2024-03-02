#!/bin/bash
set -xe

PROJECT_DIR=`pwd`
SOURCE_FILE="main.c utils.c parser.c"

EXECUTABLE="Conwey_Gol"

RAYLIB_LIB_PATH="./raylib/src/libraylib.a"
# Path to raylib includes
RAYLIB_INCLUDE_PATH="./raylib/src/"

# Check if raylib library is compiled
if [ ! -d "$RAYLIB_LIB_PATH" ]; then
    echo "Raylib library not found. Compiling raylib..."
    cd $RAYLIB_INCLUDE_PATH
    make PLATFORM=PLATFORM_DESKTOP
fi

CC=gcc
CFLAGS="-lraylib -lm -lpthread -ldl -lX11 -O3 -s  -Qn -O3"

cd $PROJECT_DIR
# Compile and link the code
$CC -Wl,--gc-sections -Wl,--print-gc-sections   -o $EXECUTABLE $SOURCE_FILE  -I. -I$RAYLIB_INCLUDE_PATH -L$RAYLIB_INCLUDE_PATH $CFLAGS
