#!/bin/bash

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE/"
CMAKE_BUILD_DIR="$BUILD_DIR/cmake"

echo -e "\n#################"
echo -e "##\tBuild $BUILD_TYPE"
echo -e "#################\n"

LOCAL_INSTALL_DIR=${LOCAL_INSTALL_DIR:-"$BUILD_DIR/dist"}
export cmocka_mocks_DIR="$LOCAL_INSTALL_DIR/usr/local/lib/cmake/cmocka_mocks/"
export cmocka_extensions_DIR="$LOCAL_INSTALL_DIR/usr/local/lib/cmake/cmocka_extensions"

cmake -B $CMAKE_BUILD_DIR $BASE_DIR -DCMAKE_BUILD_TYPE=$BUILD_TYPE -G Ninja
ninja -v -C $CMAKE_BUILD_DIR -j$(nproc) doc

mv $CMAKE_BUILD_DIR/doc $BUILD_DIR
