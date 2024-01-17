#!/bin/bash
set -eou pipefail

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
BUILD_TYPE="${1:-Debug}"
. "$BASE_DIR/ci/common_names.sh"

rm -rf $RESULT_DIR/smoketest_results
mkdir -p $RESULT_DIR/smoketest_results

if [ "${CI:-false}" = true ]; then
    export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR:-"$(realpath "${BUILD_DIR}/tmp")"}"
fi

export ENABLED_TESTS="${2:-""}"
export DISABLED_TESTS=""
export SMOKETEST_ENABLE_COMPILE_TESTS="YES"
export LD_LIBRARY_PATH="$BASE_DIR/build/deps/lib"
BASE_DIR=$BASE_DIR BUILD_TYPE=$BUILD_TYPE \
$BASE_DIR/test/smoketest/smoketest.sh | \
tee "$RESULT_DIR/smoketest_results/smoktests.log"

#tapview > $BUILD_DIR/result/smoketest_results/summary < $BUILD_DIR/result/smoketest_results/log
