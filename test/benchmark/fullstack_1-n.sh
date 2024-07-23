#!/bin/bash

BASE_DIR=$(dirname $0)/../..
export BUILD_TYPE="${BUILD_TYPE-Debug}"
export BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE/"
export DIST_DIR=${DIST_DIR-"${BUILD_DIR}/dist"}

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH-"${DIST_DIR}/usr/local/lib/"}
export PATH="${PATH}:${DIST_DIR}/usr/local/bin/"
export BENCHMARK_DIR=${BENCHMARK_DIR-"$(dirname $0)"}
export BENCHMARK_RESULT_DIR=${BENCHMARK_RESULT_DIR-"$BUILD_DIR/result/benchmark_results"}

export ELOS_SYSLOG_PATH=${ELOS_SYSLOG_PATH-"/tmp/elosd.syslog.socket"}
export ELOS_KMSG_FILE=${ELOS_KMSG_FILE-"/tmp/elosd.kmsg"}
export ELOS_CLIENT_PATH=${ELOS_SCANNER_PATH-"$DIST_DIR/usr/local/lib/elos/client"}
export ELOS_SCANNER_PATH=${ELOS_SCANNER_PATH-"$DIST_DIR/usr/local/lib/elos/scanner"}
export ELOS_BACKEND_PATH=${ELOS_BACKEND_PATH-"$DIST_DIR/usr/local/lib/elos/backend"}
export ELOS_STORAGE_BACKEND_JSONBACKEND_FILE=${ELOS_STORAGE_BACKEND_JSONBACKEND_FILE-"/tmp/elosd_event.log"}
export ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH-"$BENCHMARK_DIR/config.json"}

ELOSD_PID=""
function start_elosd() {
    echo "Start elosd..."
    elosd > $RESULT_DIR/elosd.log 2>&1 &
    ELOSD_PID=$!

    echo "wait for elosd..."
    sleep 1s
}

function stop_elosd() {
    echo "Stop elosd ($ELOSD_PID) ..."
    kill -2 $ELOSD_PID
    wait $ELOSD_PID > /dev/null 2>&1
    echo "done ($?)"
}

SUBSRCIBER_PIDS=""

function spawn_subscriber {
    local PUBLISHER=$1
    local FILTERRULE=$2
    local SUBSCRIBER=$3
    local RESULT_DIR=$4

    local i
    for i in `seq 1 $SUBSCRIBER`; do
        elosc -s "$FILTERRULE" -r 100 > "${RESULT_DIR}/sub_${i}_on_${PUBLISHER}.log" &
        SUBSRCIBER_PIDS="$SUBSRCIBER_PIDS $!"
    done
}

function join_subscriber {
    local i
    for i in $SUBSRCIBER_PIDS; do
        echo "send sigint to $i"
        kill -2 $i &
    done
    echo "wait for subscriber: $SUBSRCIBER_PIDS"
    wait $SUBSRCIBER_PIDS
}

PUBLISHER_PIDS=""

function spawn_publisher {
    local PUBLISHER=$1
    local MESSAGES=$2

    local i
    for i in `seq 1 $PUBLISHER`; do
        local EVENT="{\"source\":{\"appName\":\"benchmark_$i\"},\"payload\":[$(date "+%s,%N")]}"
        elosc -p $EVENT -c $MESSAGES &
        PUBLISHER_PIDS="$PUBLISHER_PIDS $!"
    done
    echo "wait for publisher: $PUBLISHER_PIDS"
    wait $PUBLISHER_PIDS
}

function runBenchmark {
    local MESSAGES=$1
    local PUBLISHER=$2
    local SUBSCRIBER=$3

    start_elosd

    local i
    for i in `seq 1 $PUBLISHER`; do
        local FILTERRULE=".event.source.appName 'benchmark_${i}' STRCMP"
        spawn_subscriber $i "$FILTERRULE" $SUBSCRIBER $RESULT_DIR
    done

    echo "run benchmark"
    spawn_publisher $PUBLISHER $MESSAGES $RESULT_DIR
    sleep 4
    join_subscriber

    stop_elosd
}

function getReceiveTimestamp {
    echo "$1" |cut -f 5 -d " " |tr ":" " "
}

function startWith {
    echo "$1"|grep "^$2"
    return $?
}

function evalResults {
    local MESSAGES=$1
    local PUBLISHER=$2
    local SUBSCRIBER=$3

    local REPORT_CSV="${RESULT_DIR}/report_${MESSAGES}_${SUBSCRIBER}_${PUBLISHER}.csv"

    echo "send seconds, send nseconds, dispatch seconds, dispatch nseconds, receive seconds, receive nseconds" > $REPORT_CSV

    local pub
    for pub in `seq 1 $PUBLISHER`; do
        local sub
        for sub in `seq 1 $SUBSCRIBER`; do
            local packages=$(grep -A1 "  new data " "${RESULT_DIR}/sub_${sub}_on_${pub}.log")

            local receive_sec=0
            local receive_nsec=0
            local line
            OLD_IFS=$IFS
            export IFS=$'\n'
            for line in $packages; do
                echo "eval line : $line"
                startWith "$line" "  new data"
                if [ $? == 0 ];then
                    timestamp=$(getReceiveTimestamp $line)
                    receive_sec=$(echo $timestamp | jq '.[0]')
                    receive_nsec=$(echo $timestamp | jq '.[1]')
                else
                    echo $line | jq -r ".[] | [ (.payload|fromjson[0]), (.payload|fromjson[1]), .date[0], .date[1], $receive_sec, $receive_nsec] | @csv" >> $REPORT_CSV
                fi
            done
        export IFS=$OLD_IFS
        done
    done
}

mkdir -p $BENCHMARK_RESULT_DIR

for pubs in `seq 1 2`; do # 2 5 10 100; do
    for subs in 2 5 10; do # 100; do
        MESSAGES=100
        RESULT_DIR="$BENCHMARK_RESULT_DIR/benchmark_${MESSAGES}_${subs}_${pubs}"
        BENCHMARK_LOG="$RESULT_DIR/benchmark_${subs}_${pubs}.log"

        rm -fr $RESULT_DIR
        mkdir -p $RESULT_DIR

        echo "## run for $subs subscribers , $pubs publishers"
        time runBenchmark $MESSAGES $pubs $subs > $BENCHMARK_LOG 2>&1
        echo "## evaluate results"
        time evalResults $MESSAGES $pubs $subs >> $BENCHMARK_LOG 2>&1
    done
done
