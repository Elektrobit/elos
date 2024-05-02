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
TARGET_IP=$(grep "${TARGET_NAME}" /etc/hosts | cut -f 1)

run_all()
{
  for dir in $(ls -d "$TEST_SOURCE"/*/); do
      if [ "${dir#*"$TEST_DIR"}" != "$dir" ]; then
          TEST_NAME=$(echo $dir | rev | cut -d '/' -f 2 | rev)
          echo "\nStart test suite $dir"
          robot --variablefile="$VARIABLE_FILE" --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
                --output="$TEST_NAME" --report="$TEST_NAME" "$dir" || true
      fi
  done
  rebot --output "${TEST_OUTPUT}/elos.xml" \
      --outputdir "${TEST_OUTPUT}" \
      "${TEST_OUTPUT}/*/*.xml"
}

run_suite()
{
  if [ ! -d "$1" ]; then 
      TEST_NAME=$(echo "$1" | rev | cut -d '/' -f 2 | rev)
      printf '\nStart test suite %-s\n' "$1"
      robot --variablefile="$VARIABLE_FILE" --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
            --output="$TEST_NAME" --report="$TEST_NAME" "$1" || true
  else
      echo "provided path is not a .robot suite"
  fi
}

run_module()
{
  if [ -d "$1" ]; then 
      TEST_NAME=$(echo "$1" | rev | cut -d '/' -f 2 | rev)
      printf '\nStart test module %-s\n' "$1"
      robot --variablefile="$VARIABLE_FILE" --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
            --output="$TEST_NAME" --report="$TEST_NAME" "$1" || true
  else
      echo "provided path is not a directory"
  fi
}

run_case()
{
  if [ -e "$2" ]; then 
      if grep -q "$1" "$2"; then 
          TEST_NAME=$(echo "$2" | rev | cut -d '/' -f 2 | rev)
          printf '\nStart test case %-s in suite %-s\n' "$1" "$2"
          robot --variablefile="$VARIABLE_FILE" --outputdir="$TEST_OUTPUT"/"$TEST_NAME" \
                --output="$TEST_NAME" --report="$TEST_NAME" --test "$1" "$2"
      else
          echo "test case not in given test suite"
      fi
  else
      echo "test suite does not exist"
  fi
}

print_help()
{
  echo
  echo "Usage: $0 --all : run all tests in the integartion test directory"
  echo "Usage: $0 --suite <path to test suite> : run all tests in the given test suite"
  echo "Usage: $0 --module <path to test module dir> : run all tests in the given test module directory"
  echo "Usage: $0 --case <test-case> <path to test suite> : run given test case in the given test suite"
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
       if [ -z "$2" ] || [ -z "$3" ]; then
           echo "Test case or test suite not provided"
           exit 1
       else
           run_case "$2" "$3"
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
