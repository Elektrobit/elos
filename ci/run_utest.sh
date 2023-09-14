#!/bin/bash
set -e -u -o pipefail

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}
PROJECT_NAME=$(basename $BASE_DIR)

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE/cmake"

if [ ! -d $BUILD_DIR ]; then
    $CMD_PATH/build.sh $BUILD_TYPE
fi

cd $BUILD_DIR
ctest --output-on-failure --force-new-ctest-process --verbose --output-junit junit.xml --no-compress-output


TEST_LOG_FILE="$BUILD_DIR/Testing/Temporary/LastTest.log"
SKIPPED_TESTS=$(sed -n -e '/^# skip/p' $TEST_LOG_FILE | wc -l)
if [ $SKIPPED_TESTS -gt 0 ]; then
    echo "Skipped tests (${SKIPPED_TESTS}):"
    grep "# skip " $TEST_LOG_FILE
fi
