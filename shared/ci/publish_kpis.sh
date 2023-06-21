#!/bin/bash -e
CMD_PATH=$(cd "$(dirname "$0")" && pwd)
BASE_DIR=${CMD_PATH%/*}
BASE_DIR=$(dirname $0)/..
BUILD_DIR=$BASE_DIR/build/Debug

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

function collect_kpis_from_csv {
    local CSV_FILE=$1
    local MEASURMENT="elos_tools.elos.message_runtime"

    echo "collect KPIS from $CSV_FILE"

    tail -n +2 $CSV_FILE | while read row; do
        local values=$(echo -en "$row" | xargs -d "," printf "send_sec=%d,send_nsec=%d,dispatch_sec=%d,dispatch_nsec=%d,recv_sec=%d,recv_nsec=%d")
        add_kpi "$MEASURMENT" "$(basename -s .csv $1)" "$values"
    done
}

rm -rf /tmp/kpis_to_send
BENCHMARK_RESULT_DIR=$BUILD_DIR/result/benchmark_results
for CSV_FILE in $(find  $BENCHMARK_RESULT_DIR -name "*.csv"); do
    collect_kpis_from_csv $CSV_FILE
done
push_all_kpis
rm /tmp/kpis_to_send
