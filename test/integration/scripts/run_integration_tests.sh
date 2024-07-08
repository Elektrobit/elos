#!/usr/bin/sh -e
#
# Run Robot tests using this script.
#

CMD_PATH="$(realpath "$(dirname "$0")")"
INTEGRATION_DIR=${INTEGRATION_DIR-${CMD_PATH%/*}}
BASE_DIR="$(realpath "$CMD_PATH/../../..")"
TEST_DIR=${INTEGRATION_DIR}/tests
VARIABLE_FILE="${VARIABLE_FILE-${INTEGRATION_DIR}/resources/robot_variables.py}"

. "$BASE_DIR/ci/common_names.sh"

TEST_OUTPUT="${TEST_OUTPUT-${RESULT_DIR}/integration}"
TARGET_NAME="${TARGET_NAME-elos-target}"
TARGET_IP=$(grep "${TARGET_NAME}" /etc/hosts | cut -f 1)

run_all()
{
  for dir in $(ls -d "$TEST_DIR"/*/); do
      TEST_NAME=$(echo $dir | rev | cut -d '/' -f 2 | rev)
      echo "\nStart test suite $dir"
      robot \
          --listener RetryFailed:1 \
          --randomize all \
          --variablefile="$VARIABLE_FILE" \
          --pythonpath="$INTEGRATION_DIR" \
          --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
          --output="$TEST_NAME" \
          --report="$TEST_NAME" "$dir" || true
  done
  rebot --output "${TEST_OUTPUT}/elos.xml" \
      --outputdir "${TEST_OUTPUT}" \
      "${TEST_OUTPUT}/*/*.xml"
}

run_suite()
{
  TEST_SUITE=$(find ${TEST_DIR} -type f -name ${1})
  if [ ! -d "$TEST_SUITE" ]; then
      TEST_NAME=$(echo "${TEST_SUITE}" | rev | cut -d '/' -f 2 | rev)
      printf '\nStart test suite %-s\n' "${TEST_SUITE}"
      robot \
          --variablefile="$VARIABLE_FILE" \
          --pythonpath="$INTEGRATION_DIR" \
          --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
          --output="$TEST_NAME" \
          --report="$TEST_NAME" "${TEST_SUITE}" || true
  else
      echo "provided path is not a .robot suite"
  fi
}

run_module()
{
  if [ -d "${1}" ]; then
      TEST_NAME=$(echo "${1}" | rev | cut -d '/' -f 2 | rev)
      printf '\nStart tests module %-s\n' "${1}"
      robot \
          --variablefile="$VARIABLE_FILE" \
          --pythonpath="$INTEGRATION_DIR" \
          --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
          --output="$TEST_NAME" \
          --report="$TEST_NAME" "${1}" || true
  else
      echo "provided path is not a directory"
  fi
}

run_case()
{
  TEST_SUITES=$(grep -rlI "$1" ${TEST_DIR})
  for suite in ${TEST_SUITES}; do
      if [ -e "${suite}" ]; then
          TEST_NAME=$(echo "${suite}" | rev | cut -d '/' -f 2 | rev)
          printf '\nStart test case %-s in suite %-s\n' "$1" "${suite}"
          robot \
              --variablefile="$VARIABLE_FILE" \
              --pythonpath="$INTEGRATION_DIR" \
              --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
              --output="$TEST_NAME" \
              --report="$TEST_NAME" \
              --test "$1" "${suite}"
      else
          echo "test case not found in test suite"
      fi
  done
}

print_help()
{
  echo
  echo "Usage: $0 --all : run all tests in the integration test directory"
  echo "Usage: $0 --suite <test suite name> : run all tests in the given test suite"
  echo "Usage: $0 --module <path to module > : run all tests in the given test module path"
  echo "Usage: $0 --case <test-case name> : run given test case in the given test suite"
  echo "Usage: $0 --help : show this help"
  echo
}

if [ $# -lt 1 ]; then
    print_help
    exit 1
fi

if [ -z "$TARGET_IP" ]; then
    TARGET_IP=$(ping -c 2 "${TARGET_NAME}" | awk -F '[()]' '/PING/ { print $2}')
fi

if [ -e "${VARIABLE_FILE}" ]; then
    if grep -q "TARGET_IP" "${VARIABLE_FILE}"; then
        sed -i "/TARGET_IP/c\TARGET_IP = \"$TARGET_IP\"" "$VARIABLE_FILE"
    fi
else
    echo "robot_variables file does not exist"
    exit 1
fi

case $1 in
    --all)
       run_all
       ;;

     --suite)
       if [ -z "$2" ]; then
           echo "Test suite not provided"
       else
           run_suite "$2"
       fi
       ;;

    --module)
       if [ -z "$2" ]; then
           echo "Module directory not provided"
           exit 1
       else
           run_module "$2"
       fi
       ;;

    --case)
       if [ -z "$2" ]; then
           echo "Test case or test suite not provided"
           exit 1
       else
           run_case "$2"
       fi
       ;;

    --help)
       print_help
       exit 0
       ;;

    *)
       print_help
       exit 1
       ;;
esac
