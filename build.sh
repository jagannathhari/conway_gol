set -xe

PROJECT_DIR=`pwd`
SOURCE_FILE="./src/main.c ./src/utils.c ./src/parser.c"

BUILD_DIR="./build"
EXECUTABLE="$BUILD_DIR/Conwey_Gol"

RAYLIB_LIB_PATH="./raylib/src/libraylib.a"
# Path to raylib includes
RAYLIB_INCLUDE_PATH="./raylib/src/"
INCLUDE_PATH="./src/include/"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir build
fi
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
$CC -Wl,--gc-sections -Wl,--print-gc-sections  -o $EXECUTABLE $SOURCE_FILE  -I$INCLUDE_PATH -I. -I$RAYLIB_INCLUDE_PATH -L$RAYLIB_INCLUDE_PATH $CFLAGS
