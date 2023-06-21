#!/bin/bash
set -eu

CMDPATH=${0%/*}
BASE_DIR=$(realpath $CMDPATH/../..)
SOURCE_DIR=$BASE_DIR/safu/src
BUILD_DIR=$(cd ${BUILD_DIR-$BASE_DIR/build/Release} && pwd)
ASMCOV_DIR=${ASMCOV_DIR-"./"}

TEXT_RED="\033[31m"
TEXT_GREEN="\033[32m"
TEXT_YELLOW="\033[33m"
TEXT_RESET="\033[m"

export COVERAGE_DIR=${COVERAGE_DIR-"$(dirname $0)"}
export COVERAGE_RESULT_DIR=${COVERAGE_RESULT_DIR-"$BUILD_DIR/../result/coverage_results"}

function is_build_with_asan {
  test x != x"$(ldd $1 | grep libasan)"
}

function run_test {
  local METHOD="$1"
  local BINARY_UNDER_TEST="$2"
  local TEST_NAME=$(basename $BINARY_UNDER_TEST)
  local RESULT_DIR=$COVERAGE_RESULT_DIR/$TEST_NAME

  mkdir -p $RESULT_DIR

  local PERF_DATA_FILE="$RESULT_DIR/$TEST_NAME.data"
  local PERF_TRACE_FILE="$RESULT_DIR/$TEST_NAME.trace"

  if is_build_with_asan $BINARY_UNDER_TEST; then
    echo "WARNING: code is instrumented by address sanitizer, asmcov results
    will probably reveal more branches then expected from the plain source
    code!" | tee -a $RESULT_DIR/asmcov.log
  fi
  echo "##### Coverage for $METHOD"
  LD_BIND_NOW=1 perf record -o $PERF_DATA_FILE --event intel_pt//u ${BINARY_UNDER_TEST}
  perf script -i $PERF_DATA_FILE --itrace=i0 --fields dso,sym,ip,insn,insnlen --show-mmap-events > $PERF_TRACE_FILE
  cd $RESULT_DIR
  set +e -x
  asmcov ${BINARY_UNDER_TEST} ${METHOD} $PERF_TRACE_FILE 2>&1 | tee -a $RESULT_DIR/asmcov.log
  set -e +x
  cd -
}

function print_short_report {
  local UTESTS=$1

  echo "#################################################"
  echo "## Short report"
  echo "#################################################"

  for utest in $UTESTS; do
    local TEST_NAME=$(basename $utest)
    local RESULT_DIR=$COVERAGE_RESULT_DIR/$TEST_NAME

    set +e
    local COVERAGE=$(grep "Branch coverage:" $RESULT_DIR/asmcov.log)
    set -e
    case "$COVERAGE" in
      "Branch coverage:	100%")
        echo -e "$TEXT_GREEN ${COVERAGE##*:} $TEXT_RESET $TEST_NAME"
        ;;
      "Branch coverage:	0%")
        echo -e "$TEXT_RED ${COVERAGE##*:} $TEXT_RESET $TEST_NAME"
        ;;
      "Branch coverage:"*)
        echo -e "$TEXT_YELLOW ${COVERAGE##*:} $TEXT_RESET $TEST_NAME"
        ;;
      *)
        echo -e "$TEXT_RED \t0% $TEXT_RESET $TEST_NAME"
        ;;
    esac
  done
}

if [ ! -d "$BUILD_DIR/test" ]; then
  echo "No test directory found in $BUILD_DIR. Run $BASE_DIR/<project_name>/ci/build.sh or set BUILD_DIR."
  exit 1
fi

rm -rf $COVERAGE_RESULT_DIR
mkdir -p $COVERAGE_RESULT_DIR

if [ $# -eq 0 ]; then
  SEARCH_PATTERN="-name test_*_utest"
else
  SEARCH_PATTERN=" -name test_*$1*_utest"
  shift
  for i in $@; do
    SEARCH_PATTERN="${SEARCH_PATTERN} -o -name test_*$i*_utest"
  done
fi

UTESTS=$(find $BUILD_DIR/test/ -executable -type f $SEARCH_PATTERN)

for utest in $UTESTS; do
  TMP_STRING=safu${utest##*_safu}
  METHODNAME=${TMP_STRING%%_utest}
  set +e
  objdump ${utest} -t | grep "__genuine_"${METHODNAME}
  re=$?
  set -e
  if [ ${re} -eq 0 ]; then
    run_test "__genuine_"${METHODNAME}  $utest
  else
    run_test ${METHODNAME}  $utest
  fi
done

print_short_report "$UTESTS" | tee -a $COVERAGE_RESULT_DIR/asmcov_short_report.log
