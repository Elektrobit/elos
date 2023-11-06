#!/usr/bin/sh
#
# Code linter for robot test and test resources.
#

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/../../..")"

. "$BASE_DIR/ci/common_names.sh"

BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
INTEGRATION_TEST_DIR="${BASE_DIR}/test/integration"
RESULT_DIR="${BUILD_DIR}/result"
LINT_CONFIG_DIR="${INTEGRATION_TEST_DIR}/scripts"
LINT_CONFIG=${LINT_CONFIG_DIR}/robot_linter_config.txt
TEST_OUTPUT="${RESULT_DIR}/integration/robot_lint"

RC_CHECK=0

static_code_check()
{
  echo "Static Code check"
 
  robocop -o ${TEST_OUTPUT}/robot_lint.log -A ${LINT_CONFIG} $INTEGRATION_TEST_DIR 

  RC_CHECK=$(echo $?)

  echo "Found "$RC_CHECK" format errors in code" 
}

check_code()
{
  echo "Checking robot test code"
  
  RC_CHECK=$(robotidy --check $INTEGRATION_TEST_DIR ; echo $?)

  if [ $RC_CHECK -ne 0 ]; then
      echo "Atleast one test/tests has formating error" 
  fi
}

fix_code()
{
  echo "Fixing robot test code"

  robotidy --line-length 100 $INTEGRATION_TEST_DIR
}

show_changes()
{
  echo "Showing suggested changes to robot test"

  robotidy --line-length 100 --no-overwrite --diff $INTEGRATION_TEST_DIR
}


print_help() 
{
    echo 
    echo "Usage: $0 --static_check <test> : Performs static code checks of given or all robot files"
    echo "Usage: $0 --check <test> : Checks all or given test for formating errors"
    echo "Usage: $0 --show <test>  : Shows suggested format changes for all or given tests with out writing them to files"
    echo "Usage: $0 --check-show <test> : Checks for formating error and shows suggested changes for correcting them with out writing them to files"
    echo "Usage: $0 --show-fix <test>   : Shows format changes for correcting format errors and writes them to all or given files"
    echo "Usage: $0 --check-fix <test>  : Checks for format error and if any fix format errors with out showing what changes are done"
    echo "Usage: $0 --help"
    echo
}

if [ $# -lt 1 ]; then
    print_help
    exit 1
fi

if [ ! -d "$TEST_OUTPUT" ]; then
   mkdir -p $TEST_OUTPUT
fi

if [ -n $1 ] && [ "$1" != "--help" ]; then
    if [ -n "$2" ]; then
        INTEGRATION_TEST_DIR=$(echo $2)
    else
        echo "Specific test suite or test not given"
        echo "All tests will be analysed"
    fi
fi

case $1 in
    --static_check)
       static_code_check $INTEGRATION_TEST_DIR
       exit $RC_CHECK
       ;;

    --check)
       check_code $INTEGRATION_TEST_DIR
       if [ $RC_CHECK -eq 0 ]; then
          echo "No changes to show"
       fi
       ;;

    --show)
       show_changes $INTEGRATION_TEST_DIR
       ;;

    --check-show)
       check_code $INTEGRATION_TEST_DIR
       if [ $RC_CHECK -ne 0 ]; then
           show_changes $INTEGRATION_TEST_DIR
       else
          echo "No changes to show"
       fi
       exit $RC_CHECK
       ;;

    --show-fix)
       show_changes $INTEGRATION_TEST_DIR
       fix_code $INTEGRATION_TEST_DIR
       exit $RC_CHECK
       ;;

    --check-fix)
       check_code $INTEGRATION_TEST_DIR
       if [ $RC_CHECK -ne 0 ]; then
           show_changes $INTEGRATION_TEST_DIR
           fix_code $INTEGRATION_TEST_DIR
       else
          echo "No changes to show or fix"
       fi
       exit 0
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
