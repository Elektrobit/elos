#!/bin/bash -exu
CMD_PATH=$(cd "$(dirname "$0")" && pwd)
BASE_DIR=${CMD_PATH%/*}
BASE_DIR=$(dirname $0)/..

BRANCH_NAME=${BRANCH_NAME-$(git branch --show-current)}

KPI_API_URL="${KPI_API_URL-http://localhost:8086/api/v2/write?org=elos&bucket=elos&precision=ns}"
KPI_API_TOKEN="${KPI_API_TOKEN-"API_token_not_defined"}"

function add_kpi {
    local MEASURMENT=$1
    local IMAGE="docker-ci"
    local TARGET="docker-ci"
    local ARCH="x86-64"
    local SCENARIO="$2"
    local VALUES=$3
    local TIMESTAMP=${4-$(date +%s%N)}
    echo "$MEASURMENT,host=${HOSTNAME},branch=${BRANCH_NAME},image=${IMAGE},target=${TARGET},arch=${ARCH},scenario=${SCENARIO} ${VALUES} $(date +%s%N)" >> /tmp/kpis_to_send
}

function push_all_kpis {
    curl -v -i "${KPI_API_URL}" \
        --header "Authorization: Token ${KPI_API_TOKEN}" \
        --header "Content-Type: text/plain; charset=utf-8" \
        --data-binary "@/tmp/kpis_to_send"
}

function collect_coverage_kpis {
    local COVERAGE_RESULT_DIR=$BASE_DIR/build/Release/result/coverage_results/

    if [ ! -d $COVERAGE_RESULT_DIR ]; then
        echo "Skip publishing coverage, no coverage results found"
    fi

    find $BASE_DIR/build/Release/cmake/src/ -type f -name "*.a" -and -not -name "*_weak.a" -exec objdump -t {} \; > /tmp/symbols.log

    PUBLIC_FUNCTIONS=$(grep "g[ ]*F .text" /tmp/symbols.log | grep -oE "[^ ]+$" || true)
    PUBLIC_FUNCTION_COUNT=$(echo $PUBLIC_FUNCTIONS | wc -w)

    LOCAL_FUNCTIONS=$(grep "l[ ]*F .text" /tmp/symbols.log | grep -oE "[^ ]+$" || true)
    LOCAL_FUNCTION_COUNT=$(echo $LOCAL_FUNCTIONS | wc -w)

    TESTED_PUBLIC_FUNCTIONS=$(find $COVERAGE_RESULT_DIR -mindepth 1 -type d -printf "%f ")
    TESTED_PUBLIC_FUNCTION_COUNT=$(echo $TESTED_PUBLIC_FUNCTIONS | wc -w)

    echo "coverage for tests : $TESTED_PUBLIC_FUNCTIONS"
    local COVERAGE=0
    for TEST_NAME in $TESTED_PUBLIC_FUNCTIONS; do
        echo "--> $TEST_NAME"
        local RESULT_DIR=$COVERAGE_RESULT_DIR/$TEST_NAME
        if [ -f $RESULT_DIR/asmcov.log ]; then
          local VALUE=$(grep "Branch coverage:" $RESULT_DIR/asmcov.log|tr -dc 0-9)
          if [ "x$VALUE" != "x" ]; then
              COVERAGE=$((COVERAGE+VALUE))
          fi
        else
          echo "Skip $RESULT_DIR, look not like a test result dir"
        fi
    done

    if [ $TESTED_PUBLIC_FUNCTION_COUNT -gt 0 ]; then
        TESTED_COVERAGE=$((COVERAGE/TESTED_PUBLIC_FUNCTION_COUNT))
    else
        TESTED_COVERAGE=0
    fi

    if [ $PUBLIC_FUNCTION_COUNT -gt 0 ]; then
        TOTAL_COVERAGE=$((COVERAGE/PUBLIC_FUNCTION_COUNT))
    else
        TOTAL_COVERAGE=0
    fi

    echo "tested coverage: $TESTED_COVERAGE"
    echo "total coverage: $TOTAL_COVERAGE"
    add_kpi "elos_tools.safu.coverage" "build/Release/result" "public_functions=$PUBLIC_FUNCTION_COUNT,tested_public_functions=$TESTED_PUBLIC_FUNCTION_COUNT,local_functions=$LOCAL_FUNCTION_COUNT,average_coverage=$TOTAL_COVERAGE,average_tested_coverage=$TESTED_COVERAGE"

    rm /tmp/symbols.log
}

rm -rf /tmp/kpis_to_send

collect_coverage_kpis

push_all_kpis
rm /tmp/kpis_to_send
