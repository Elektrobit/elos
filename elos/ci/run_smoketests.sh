#!/bin/bash
set -eou pipefail

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"

rm -rf $BUILD_DIR/result/smoketest_results
mkdir -p $BUILD_DIR/result/smoketest_results

export ENABLED_TESTS="${2:-""}"
export DISABLED_TESTS=""
BASE_DIR=$BASE_DIR BUILD_TYPE=$BUILD_TYPE \
$BASE_DIR/test/smoketest/smoketest.sh | \
tee $BUILD_DIR/result/smoketest_results/log

#tapview > $BUILD_DIR/result/smoketest_results/summary < $BUILD_DIR/result/smoketest_results/log
