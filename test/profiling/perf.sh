#!/bin/bash
set -e -u

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/../..")"

BUILD_TYPE=${BUILD_TYPE:-"Coverage"}

. "$BASE_DIR/ci/common_names.sh"

SMOKETEST_DIR=${SMOKETEST_DIR-"$BASE_DIR/test/smoketest"}
PERF_RESULTS_PATH=${PERF_RESULTS_PATH:-$RESULT_DIR/perf_results}
PERF_RESULT_PATH=${PERF_RESULT_PATH:-$PERF_RESULTS_PATH/$(date "+%Y%m%d_%H%M%S")}

CMAKE_PARAM=${CMAKE_PARAM:-""}

ELOSD_BINARY=${ELOSD_BINARY:-$BUILD_DIR/dist/usr/local/bin/elosd}
export ELOS_SYSLOG_PATH=${ELOS_SYSLOG_PATH-"/tmp/elosd.syslog.socket"}
export ELOS_KMSG_FILE=${ELOS_KMSG_FILE-"/tmp/elosd.kmsg"}
export ELOS_SCANNER_PATH=${ELOS_SCANNER_PATH-"$BUILD_DIR/src/scanner"}
export ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH-"$SMOKETEST_DIR/config.json"}
export ELOS_STORAGE_BACKEND_JSON_FILE=${ELOS_STORAGE_BACKEND_JSON_FILE-"/tmp/elosd_event.log"}

PERF=${PERF:-perf}
PERF_RECORD_PARAM=${PERF_PARAM:-record --user-callchains -g --call-graph dwarf}
PERF_SCRIPT_PARAM=${PERF_PARAM:-script}
PERF_PROFILE_COMMAND=${PERF_PROFILE_COMMAND:-$ELOSD_BINARY}
PERF_OUTPUT_TXT=${PERF_OUTPUT_TXT:-$PERF_RESULT_PATH/perf_output.txt}
PERF_CAP_FILE=${PERF_CAP_FILE:-/proc/sys/kernel/perf_event_paranoid}
PERF_CAP_BACKUP=${PERF_CAP_BACKUP:-$PERF_RESULTS_PATH/$(basename $PERF_CAP_FILE).bak}
PERF_CAP_LEVEL=${PERF_CAP_LEVEL:-3}

function perf_clean {
    "$BASE_DIR/ci/clean.sh"
}

function perf_build {
    perf_clean
    "$BASE_DIR/ci/build.sh" "$BUILD_TYPE"
}

function perf_elosd {
    if [ ! -f "$ELOSD_BINARY" ]
    then
        echo "elosd binary not found, run this script with parameter 'build' first"
        exit 1
    fi

    mkdir -p $PERF_RESULT_PATH
    cd $PERF_RESULT_PATH

    echo $PERF $PERF_RECORD_PARAM $PERF_PROFILE_COMMAND
    $PERF $PERF_RECORD_PARAM $PERF_PROFILE_COMMAND &
    ELOSD_PID=$!
    sleep 5s
    echo "Stopping elosd ..."
    kill -15 $ELOSD_PID &
    wait $ELOSD_PID || true

    echo $PERF $PERF_SCRIPT_PARAM
    $PERF $PERF_SCRIPT_PARAM > $PERF_OUTPUT_TXT
    echo "Logged perf statistics in file '$PERF_OUTPUT_TXT'"
    cd -
}

function perf_setcap {
    if [ -f "$PERF_CAP_BACKUP" ]
    then
        echo "file '$PERF_CAP_BACKUP' already exists, use 'clearcap' to remove it if wanted"
    else
        elosd_usergroup=$(stat --printf='%U:%G' $BASE_DIR)
        mkdir -p $PERF_RESULTS_PATH
        cat $PERF_CAP_FILE > $PERF_CAP_BACKUP
        chown -R $elosd_usergroup $BUILD_DIR
        echo "Saved old capability level to file '$PERF_CAP_BACKUP'"
    fi
    # Allow user space perf profiling (needs root rights)
    echo $PERF_CAP_LEVEL > $PERF_CAP_FILE
}

function perf_restorecap {
    if [ -f "$PERF_CAP_BACKUP" ]
    then
        cat $PERF_CAP_BACKUP > $PERF_CAP_LEVEL
        rm $PERF_CAP_BACKUP
    else
        echo "file '$PERF_CAP_BACKUP' not found, cannot restore capability level"
        exit 1
    fi
}

function getlastresultpath {
    local -n lastresultpath=$1
    oldpath=$(pwd)
    lastresultpath=$(find $PERF_RESULTS_PATH -mindepth 1 -type d | sort -r | tail -n 1)
}

function perf_report {
    getlastresultpath last_result
    result_file="$last_result/perf.data"
    echo "Using file '$result_file'"
    perf report -i $result_file
}

function perf_hotspot {
    getlastresultpath last_result
    result_file="$last_result/perf.data"
    echo "Using file '$result_file'"
    hotspot $result_file
}

function perf_help {
    echo
    echo "Usage: $0 <build|elosd|setcap|restorecap|hotspot|report>"
    echo
}

if [ $# -lt 1 ]
then
    perf_help
    exit 0
fi

case $1 in

    build)
        perf_build
        ;;
    
    setcap)
        perf_setcap
        ;;

    restorecap)
        perf_restorecap
        ;;

    clean)
        perf_clean
        ;;

    elosd)
        perf_elosd
        ;;

    report)
        perf_report
        ;;

    hotspot)
        perf_hotspot
        ;;

    *)
        perf_help
        ;;
esac
