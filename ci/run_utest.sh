#!/bin/bash
set -e -u -o pipefail

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
BUILD_TYPE="${1:-Debug}"
. "$BASE_DIR/ci/common_names.sh"

if [ ! -d $BUILD_DIR ]; then
    $CMD_PATH/build.sh $BUILD_TYPE
fi

cd $BUILD_DIR/cmake
ctest --output-on-failure --force-new-ctest-process --verbose --output-junit junit.xml --no-compress-output


TEST_LOG_FILE="$BUILD_DIR/cmake/Testing/Temporary/LastTest.log"
SKIPPED_TESTS=$(sed -n -e '/^# skip/p' $TEST_LOG_FILE | wc -l)
if [ $SKIPPED_TESTS -gt 0 ]; then
    echo "Skipped tests (${SKIPPED_TESTS}):"
    grep "# skip " $TEST_LOG_FILE
fi
