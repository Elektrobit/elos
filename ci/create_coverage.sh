#!/bin/bash

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}
BUILD_DIR="$BASE_DIR/build/Release/cmake"

$CMD_PATH/build.sh Release

BUILD_DIR="$BUILD_DIR" $BASE_DIR/test/coverage/run_asmcov.sh
STATUS=$?

find $BUILD_DIR/../result/coverage_results -name "*.trace" -delete

exit $?
