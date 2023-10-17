#!/usr/bin/sh -e
#
# Run Robot tests using this script.
#

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/../../..")"

. "$BASE_DIR/ci/common_names.sh"

TEST_SOURCE=${TEST_SOURCE-${CMD_PATH%/*}}
VARIABLE_FILE="${VARIABLE_FILE-${TEST_SOURCE}/robot_variables.py}"
TEST_OUTPUT="${TEST_OUTPUT-${RESULT_DIR}/integration}"
TEST_DIR="_test"
TARGET_NAME="${TARGET_NAME-${PROJECT}-target}"
TARGET_IP=$(grep ${TARGET_NAME} /etc/hosts | cut -f 1)

if [ -z $TARGET_IP ]; then
    TARGET_IP=$(ping -c 2 "${TARGET_NAME}" | awk -F '[()]' '/PING/ { print $2}')
fi

if [ -e ${VARIABLE_FILE} ]; then
    if [ -n "$(grep "TARGET_IP" ${VARIABLE_FILE})" ]; then
        sed -i "/TARGET_IP/c\TARGET_IP = \"$TARGET_IP\"" $VARIABLE_FILE
    fi
else
    echo "robot_variables file does not exist"
    exit 1
fi

for dir in $(ls -d "$TEST_SOURCE"/*/); do
    if [ "${dir#*$TEST_DIR}" != "$dir" ]; then
        TEST_NAME=$(echo $dir | rev | cut -d '/' -f 2 | rev)
        echo "\nStart test suite $dir"
        robot --variablefile="$VARIABLE_FILE" --outputdir="$TEST_OUTPUT"/"$TEST_NAME" --output="$TEST_NAME" --report="$TEST_NAME" $dir || exit_status=$?
    fi
done

exit "${exit_status:-0}"
