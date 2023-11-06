#!/bin/bash -e
#
# run code linting for robot integration test
#

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"

BUILD_TYPE="${1:-Debug}"

. "$BASE_DIR/ci/common_names.sh"

export BUILD_TYPE
bash "${BASE_DIR}"/test/integration/scripts/robot_code_lint.sh --static_check

