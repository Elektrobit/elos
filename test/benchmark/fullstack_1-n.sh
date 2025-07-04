#!/bin/sh

CMDPATH="$(realpath "$(dirname "$0")")"
. "${CMDPATH}/benchmark_test_env.sh"

ELOSD_PID=""
start_elosd() {
    echo "Start elosd..."
    elosd > "${RESULT_DIR}/elosd.log" 2>&1 &
    ELOSD_PID=$!

    echo "wait for elosd..."
    sleep 1s
}

stop_elosd() {
    echo "Stop elosd ($ELOSD_PID) ..."
    kill -2 $ELOSD_PID
    wait $ELOSD_PID > /dev/null 2>&1
    echo "done ($?)"
}

SUBSRCIBER_PIDS=""

spawn_subscriber() {
    PUBLISHER=$1
    FILTERRULE=$2
    SUBSCRIBER=$3
    RESULT_DIR=$4

    for i in $(seq 1 "${SUBSCRIBER}"); do
        elosc -s "$FILTERRULE" -r 100 > "${RESULT_DIR}/sub_${i}_on_${PUBLISHER}.log" &
        SUBSRCIBER_PIDS="$SUBSRCIBER_PIDS $!"
    done
}

join_subscriber() {
    for i in $SUBSRCIBER_PIDS; do
        echo "send sigint to $i"
        kill -2 "$i" &
    done
    echo "wait for subscriber: $SUBSRCIBER_PIDS"
    wait "${SUBSRCIBER_PIDS}"
}

PUBLISHER_PIDS=""

spawn_publisher() {
    PUBLISHER=$1
    MESSAGES=$2

    for i in $(seq 1 "${PUBLISHER}"); do
        elosc -p "{\"source\":{\"appName\":\"benchmark_$i\"},\"payload\":[$(date "+%s,%N")]}" -c "${MESSAGES}" &
        PUBLISHER_PIDS="$PUBLISHER_PIDS $!"
    done
    echo "wait for publisher: $PUBLISHER_PIDS"
    wait "${PUBLISHER_PIDS}"
}

runBenchmark() {
    MESSAGES=$1
    PUBLISHER=$2
    SUBSCRIBER=$3

    start_elosd

    for i in $(seq 1 "${PUBLISHER}"); do
        FILTERRULE=".event.source.appName 'benchmark_${i}' STRCMP"
        spawn_subscriber "${i}" "${FILTERRULE}" "${SUBSCRIBER}" "${RESULT_DIR}"
    done

    echo "run benchmark"
    spawn_publisher "${PUBLISHER}" "${MESSAGES}" "${RESULT_DIR}"
    sleep 4
    join_subscriber

    stop_elosd
}

getReceiveTimestamp() {
    echo "$1" |cut -f 3 -d " " |tr ":" " "
}

startWith() {
    echo "$1"|grep "^$2"
    return $?
}

evalResults() {
    MESSAGES=$1
    PUBLISHER=$2
    SUBSCRIBER=$3

    REPORT_CSV="${RESULT_DIR}/report_${MESSAGES}_${SUBSCRIBER}_${PUBLISHER}.csv"

    echo "send seconds, send nseconds, dispatch seconds, dispatch nseconds, receive seconds, receive nseconds" > "${REPORT_CSV}"

    for pub in $(seq 1 "${PUBLISHER}"); do
        for sub in $(seq 1 "${SUBSCRIBER}"); do
            packages=$(grep -A1 "  new data " "${RESULT_DIR}/sub_${sub}_on_${pub}.log")

            receive_sec=0
            receive_nsec=0
            echo "$packages" | while read -r line; do
                echo "eval line : $line"
                if startWith "$line" "new data"; then
                    timestamp=$(getReceiveTimestamp "$line")
                    receive_sec=$(echo "${timestamp}" | jq '.[0]')
                    receive_nsec=$(echo "${timestamp}" | jq '.[1]')
                else
                    echo "${line}" | jq -r ".[] | [ (.payload|fromjson[0]), (.payload|fromjson[1]), .date[0], .date[1], $receive_sec, $receive_nsec] | @csv" >> "${REPORT_CSV}"
                fi
            done
        done
    done
}

mkdir -p "${BENCHMARK_RESULT_DIR}"

for pubs in $(seq 1 2); do # 2 5 10 100; do
    for subs in 2 5 10; do # 100; do
        MESSAGES=100
        RESULT_DIR="$BENCHMARK_RESULT_DIR/benchmark_${MESSAGES}_${subs}_${pubs}"
        BENCHMARK_LOG="$RESULT_DIR/benchmark_${subs}_${pubs}.log"

        rm -fr "${RESULT_DIR}"
        mkdir -p "${RESULT_DIR}"

        echo "## run for $subs subscribers , $pubs publishers"
        runBenchmark "${MESSAGES}" "${pubs}" "${subs}" > "${BENCHMARK_LOG}" 2>&1
        echo "## evaluate results"
        evalResults "${MESSAGES}" "${pubs}" "${subs}" >> "${BENCHMARK_LOG}" 2>&1
    done
done
