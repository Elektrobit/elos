#!/bin/bash
set -eou pipefail

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"

PARAM=""
INTERACTIVE=0
DISABLED_TESTS=""
ENABLED_TESTS=""
while [ $# -gt 0 ]; do
    case ${1} in
        --interactive|-i)
            INTERACTIVE=1 ;;
        --disable|-d)
            DISABLED_TESTS="${DISABLED_TESTS} $2"
            shift ;;
        --enable|-e)
            ENABLED_TESTS="${ENABLED_TESTS} $2"
            shift ;;
        -*)
            echo "error: unknown option: $1"; exit 1 ;;
        *)
            PARAM="$PARAM ${1}" ;;
    esac
    shift
done

set -- $PARAM
BUILD_TYPE="${1:-Debug}"

. "$BASE_DIR/ci/common_names.sh"

rm -rf $RESULT_DIR/smoketest_results
mkdir -p $RESULT_DIR/smoketest_results

if [ "${CI:-false}" = true ]; then
    export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR:-"$(realpath "${BUILD_DIR}/tmp")"}"
fi

export ENABLED_TESTS="${ENABLED_TESTS}"
export DISABLED_TESTS="${DISABLED_TESTS}"
if [ "${INTERACTIVE}" -eq 0 ]; then
    BUILD_TYPE=$BUILD_TYPE \
    $BASE_DIR/test/smoketest/smoketest.sh | \
        tee "$RESULT_DIR/smoketest_results/smoktests.log"
else
    BUILD_TYPE=$BUILD_TYPE \
    $BASE_DIR/test/smoketest/smoketest_interactive.sh
fi

#tapview > $BUILD_DIR/result/smoketest_results/summary < $BUILD_DIR/result/smoketest_results/log
