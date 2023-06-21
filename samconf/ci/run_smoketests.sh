#!/bin/bash
set -eou pipefail

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE/"

export SMOKETEST_RESULT_DIR="${SMOKETEST_RESULT_DIR-$BUILD_DIR/result/smoketest_results}"
SMOKETEST_RUNNER="$BASE_DIR/test/smoketest/smoketest.sh"

rm -rf $SMOKETEST_RESULT_DIR

time $SMOKETEST_RUNNER simple_config $BUILD_TYPE
time $SMOKETEST_RUNNER signed_config $BUILD_TYPE
time $SMOKETEST_RUNNER error_signed_config $BUILD_TYPE
time $SMOKETEST_RUNNER sign_config $BUILD_TYPE
time $SMOKETEST_RUNNER genkeys $BUILD_TYPE

