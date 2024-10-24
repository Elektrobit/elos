#!/bin/bash
set -eou pipefail

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR=${CMD_PATH%/*}

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

if [ "${CI:-false}" = true ]; then
    export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR:-"$(realpath "${BUILD_DIR}/tmp")"}"
fi


export PREFIX_PATH="${BUILD_DIR}/dist/usr/local"
export LD_LIBRARY_PATH="${BASE_DIR}/build/deps/lib"
export PKG_CONFIG_PATH="${BASE_DIR}/build/deps/lib/pkgconfig"
export SMOKETEST_RESULT_DIR="${SMOKETEST_RESULT_DIR-$BUILD_DIR/result/smoketest}"
export ENABLED_TESTS="${ENABLED_TESTS}"
export DISABLED_TESTS="${DISABLED_TESTS}"

rm -rf "${SMOKETEST_RESULT_DIR}"
mkdir -p "${SMOKETEST_RESULT_DIR}"

if [ "${INTERACTIVE}" -eq 0 ]; then
    BUILD_TYPE=$BUILD_TYPE \
    $BASE_DIR/test/smoketest/smoketest.sh | \
        tee "${SMOKETEST_RESULT_DIR}/smoktests.log"
else
    BUILD_TYPE=$BUILD_TYPE \
    $BASE_DIR/test/smoketest/smoketest_interactive.sh
fi
