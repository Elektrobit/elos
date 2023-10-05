#!/bin/bash
set -eou pipefail

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
BUILD_TYPE="${1:-Debug}"
. "$BASE_DIR/ci/common_names.sh"

rm -rf $RESULT_DIR/smoketest_results
mkdir -p $RESULT_DIR/smoketest_results

export ENABLED_TESTS="${2:-""}"
export DISABLED_TESTS=""
BASE_DIR=$BASE_DIR BUILD_TYPE=$BUILD_TYPE \
$BASE_DIR/test/smoketest/smoketest.sh | \
tee "$RESULT_DIR/smoketest_results/smoktests.log"

#tapview > $BUILD_DIR/result/smoketest_results/summary < $BUILD_DIR/result/smoketest_results/log
