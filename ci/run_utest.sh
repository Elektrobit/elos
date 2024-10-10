#!/bin/bash
###############################################################################
print_info() {
    echo "
    Run the unit test suite or parts of it. Default is to run all unit
    tests for the Debug build.

    Usage: ${0} [build type] [--test-regex|-R <test name pattern>] [-h|--help]

    build type:       usually Debug or Release but can be any other build type
    --test-regex|-R:  execute all tests matching the pattern
    -h|--help:        print this help

    Examples:
    ${0} Release # run all unit test on Release build
    ${0} Release -R elosRpn # run all unit test containing elosRpn in
    the name for the Release build.
    "
}
###############################################################################
set -e -u -o pipefail

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"

TESTS_REGEX=""
PARAM=""
while [ $# -gt 0 ]; do
    case ${1} in
        --tests-regex|-R)
            TESTS_REGEX="--tests-regex ${2}"
            shift
            ;;
        -h|--help)
            print_info
            exit 0 ;;
        -*)
            echo "error: unknown option: $1"
            print_info
            exit 1 ;;
        *)
            PARAM="$PARAM ${1}" ;;
    esac
    shift
done
set -- $PARAM

BUILD_TYPE="${1:-Debug}"

. "$BASE_DIR/ci/common_names.sh"

if [ ! -d "$CMAKE_BUILD_DIR" ]; then
    "$CMD_PATH/build.sh" "$BUILD_TYPE"
fi

TEST_DIR="$CMAKE_BUILD_DIR"
mkdir -p "$RESULT_DIR/unit_test"
cd "$RESULT_DIR/unit_test"
ctest --output-on-failure --force-new-ctest-process --verbose \
    --output-junit "$RESULT_DIR/unit_test/junit.xml" \
    --no-compress-output \
    --output-log "$RESULT_DIR/unit_test/Test.log" \
    --test-dir "$TEST_DIR" \
    ${TESTS_REGEX}

cp -r "${CMAKE_BUILD_DIR}/Testing/Temporary" "${RESULT_DIR}/unit_test/"

TEST_LOG_FILE="${TEST_DIR}/Testing/Temporary/LastTest.log"
SKIPPED_TESTS=$(sed -n -e '/^# skip/p' "${TEST_LOG_FILE}" | wc -l)
if [ "${SKIPPED_TESTS}" -gt 0 ]; then
    echo "Skipped tests (${SKIPPED_TESTS}):"
    grep "# skip " "${TEST_LOG_FILE}"
fi
