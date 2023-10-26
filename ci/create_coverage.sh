#!/bin/bash

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
BUILD_TYPE=Release
. "$BASE_DIR/ci/common_names.sh"

"$CMD_PATH/build.sh" "$BUILD_TYPE"

BUILD_DIR="$BUILD_DIR/cmake" $BASE_DIR/test/coverage/run_asmcov.sh
STATUS=$?

find "$RESULT_DIR/coverage_results" -name "*.trace" -delete

exit $?
