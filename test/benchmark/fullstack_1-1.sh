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
    SUBSCRIBER=$1
    RESULT_DIR=$2

    for i in $(seq 1 "${SUBSCRIBER}"); do
        elosc -s ".event.source.appName 'benchmark_$i' STRCMP" -r 100 > "${RESULT_DIR}/sub_$i.log" &
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

    spawn_subscriber "${SUBSCRIBER}" "${RESULT_DIR}"
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
    echo "$1" | grep "^$2"
    return $?
}

evalResults() {
    MESSAGES=$1
    PUBLISHER=$2
    SUBSCRIBER=$3

    REPORT_CSV="${RESULT_DIR}/report_${MESSAGES}_${SUBSCRIBER}_${PUBLISHER}.csv"

    echo "send seconds, send nseconds, dispatch seconds, dispatch nseconds, receive seconds, receive nseconds" > "${REPORT_CSV}"

    for i in $(seq 1 "${SUBSCRIBER}"); do
        packages=$(grep -A1 "  new data " "${RESULT_DIR}/sub_$i.log")

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
}

for i in 1 2 5 10; do # 100; do
    MESSAGES=10
    PUBLISHER=$i
    SUBSCRIBER=$i
    RESULT_DIR="$BENCHMARK_RESULT_DIR/benchmark_${MESSAGES}_${SUBSCRIBER}_${PUBLISHER}"
    BENCHMARK_LOG="$RESULT_DIR/benchmark_${SUBSCRIBER}_${PUBLISHER}.log"

    rm -fr "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"

    echo "## run for $SUBSCRIBER subscribers , $PUBLISHER publishers"
    runBenchmark $MESSAGES $PUBLISHER $SUBSCRIBER > "${BENCHMARK_LOG}" 2>&1
    echo "## evaluate results"
    evalResults "${MESSAGES}" "${PUBLISHER}" "${SUBSCRIBER}" >> "${BENCHMARK_LOG}" 2>&1
done
