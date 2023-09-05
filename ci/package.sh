#!/bin/bash
set -e -u -o pipefail

CMD_PATH=$(realpath $(dirname $0))
BASE_DIR=${CMD_PATH%/*}

BUILD_TYPE="${1:-Release}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
CMAKE_BUILD_DIR="$BUILD_DIR/cmake"

cd $CMAKE_BUILD_DIR
cpack -G DEB
