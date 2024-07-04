#!/bin/bash
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
