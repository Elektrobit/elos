#!/bin/sh
# shellcheck disable=SC2317
###############################################################################
print_info() {
    SCRIPT_NAME=${0##*/}
    echo "
    Run the smoketest test suite or parts of it. Default is to run all
    smoketest tests.

    Usage: ${SCRIPT_NAME} [-h|--help]

    -h|--help:        print this help


    Examples:
    ${SCRIPT_NAME} # run all unit test on Release build
    ENABLED_TESTS="elosd syslog" ${SCRIPT_NAME} # run only elosd and syslog test
    DISABLED_TESTS="elosd syslog" ${SCRIPT_NAME} # skip elosd and syslog test

    To control which testcases are executed the following environment variables
    can be used:

    ENABLED_TESTS - spaces separated list of tests to run, default is run all
            test.
    DISABLED_TESTS - spaces separated list of tests to skip, default is run all
            test.
    SMOKETEST_ENABLE_COMPILE_TESTS - enable compile test, which check if
            neccessary header, pkg-config and cmake files to compile a basic
            program using elos libraries are gets are as expected. Default is
            off, set to any value to enable it.

    The smoketest can be configured through several environment variables. See
    smoketest_env.sh for more details about how to setup the test suite and
    about the defaults.

    $(cat $(realpath "$(dirname "${0}")/smoketest_env.sh"))
    "
}
###############################################################################

CMDPATH=$(realpath "$(dirname "$0")")

. ${CMDPATH}/smoketest_env.sh
. ${CMDPATH}/smoketest_utils.sh
. ${CMDPATH}/smoketest_log.sh


prepare_env() {
    test_name=${1?:"first parameter missing"}

    result_dir="$SMOKETEST_RESULT_DIR/${test_name}"
    if [ -e $result_dir ]
    then
        rm -rf $result_dir
    fi
    mkdir -p $result_dir

    if [ -e "$SMOKETEST_TMP_DIR" ]
    then
        rm -rf $SMOKETEST_TMP_DIR
    fi
    mkdir -p $SMOKETEST_TMP_DIR

    export RESULT_DIR=$result_dir

    setup_log "${result_dir}"

    ELOSD_PIDS=$(pgrep elosd || echo "")
    for ELOSD_PID in $ELOSD_PIDS; do
        PARENT_PID="$(get_parent_pid "${ELOSD_PID}")"
        if [ "${PARENT_PID}" -ne $$ ]; then
           log "Found elosd from other process".
           continue
       fi
       log "Existing instance of elosd found ($ELOSD_PID), terminating..."
       kill -15 $ELOSD_PID
       wait $ELOSD_PID > /dev/null
       log "done ($?)"
       sleep 2s
    done

    export ELOS_STORAGE_BACKEND_JSONBACKEND_FILE="${result_dir}/elosd_event_%count%.log"
}

run_in_source_tree() {
    [ -d "${SMOKETEST_DIR}/../../build/deps" ]
}

wait_for_file() {
    local i=0
    while [ ! -e $1 ]
    do
      i=$((i+1))
      sleep 0.1s
      if [ $i -gt 100 ]; then
         log "Error: Waiting for file $1 timed out"
         exit 124
      fi
    done

}

wait_for_elosd_socket() {
    local i=0
    while ! is_listening_on "${1}" "${ELOSD_PORT}"
    do
      i=$((i+1))
      sleep 0.1s
      if [ $i -gt 100 ]; then
         log "Error: Waiting for elosd socket timed out"
         exit 124
      fi
    done
}

wait_for_elosd_claims_running() {
    grep -q "Running..." "${1}" || \
        (tail -f -n0 "${1}"&) | timeout 5 grep -q "Running..."
}

start_dlt_mock() {
    if [ -e "${ELOS_DLT_PIPE_PATH}" ]; then
        rm -f "${ELOS_DLT_PIPE_PATH}"
    fi

    mkfifo "${ELOS_DLT_PIPE_PATH}"

    tail -f "${ELOS_DLT_PIPE_PATH}" >> /tmp/dlt_dump &
    DLT_MOCK_PID=$!
}

stop_dlt_mock() {
    kill "${DLT_MOCK_PID}"

    if [ -e "${ELOS_DLT_PIPE_PATH}" ]; then
        rm -f "${ELOS_DLT_PIPE_PATH}"
    fi
}


smoketest_elosd() {
    prepare_env "elosd"

    LOG_ELOSD="$RESULT_DIR/elosd.log"
    REAL_ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH}
    export ELOS_CONFIG_PATH=${RESULT_DIR}/test_config.json
    cp "${REAL_ELOS_CONFIG_PATH}" "${ELOS_CONFIG_PATH}"

    ELOS_DLT_PIPE_PATH="${RESULT_DIR}/dlt"
    sed -i "s,/tmp/dlt,${ELOS_DLT_PIPE_PATH}," "${ELOS_CONFIG_PATH}"
    start_dlt_mock

    log "Starting Elosd with config ${ELOS_CONFIG_PATH}"
    elosd > $LOG_ELOSD 2>&1 &
    ELOSD_PID=$!
    log "Elosd has PID ${ELOSD_PID}"
    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_file "${ELOS_DLT_PIPE_PATH}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"
    find /proc -maxdepth 1 -name $ELOSD_PID -exec kill -15 $ELOSD_PID \;
    wait $ELOSD_PID || true
    log "stopped Elosd"

    stop_dlt_mock

    export ELOS_CONFIG_PATH="${REAL_ELOS_CONFIG_PATH}"

    STRINGS="listen on: ${ELOSD_INTERFACE-"127.0.0.1"}:${ELOSD_PORT-"54323"}
hardwareid: $(cat /etc/machine-id)
log level: ${ELOS_LOG_LEVEL}
log filter: ${ELOS_LOG_FILTER-""}
scanner path: ${ELOS_SCANNER_PATH-"(null)"}
Running...
Shutting down..."
    echo "${STRINGS}" >> ${SMOKETEST_TMP_DIR}/expected_elosd_logs.txt

    local FAIL=0
    while IFS= read -r str; do

        log "look for '$str'"
        grep "$str" "${LOG_ELOSD}" > /dev/null 2>&1
        case $? in
            0)
                log "Found: '$str'"
            ;;
            1)
                log_err "Not found: '$str'"
                FAIL=$((FAIL+1))
            ;;
            *)
                log_err "Failed to check result, cancel test"
                FAIL=$((FAIL+1))
                break
            ;;
        esac
    done < ${SMOKETEST_TMP_DIR}/expected_elosd_logs.txt

    grep -e "WARNING\|ERROR" "${LOG_ELOSD}" > /dev/null 2>&1
    if [ $? -ne 1 ]; then
        FAIL=$((FAIL+1))
        log_err "Elos log contains WARNINGS or ERRORS"
    fi


    return $FAIL
}

smoketest_elosd_config_not_found() {
    prepare_env "elosd_config_not_found"

    REAL_ELOS_CONFIG_PATH=$ELOS_CONFIG_PATH
    ELOS_CONFIG_PATH=/dev

    log "Starting Elosd"
    elosd > $RESULT_DIR/elosd_config_not_set.txt 2>&1 &
    ELOSD_PID=$!
    log "Elosd has PID ${ELOSD_PID}"
    sleep 1s
    find /proc -maxdepth 1 -name $ELOSD_PID -exec kill -15 $ELOSD_PID \; &&
    wait $ELOSD_PID || true
    log "Killed Elosd"

    FAIL=0
    str="ERROR: samconfLoad"
    grep -q "$str" $RESULT_DIR/elosd_config_not_set.txt

    if [ $? -ne 0 ]; then
        log_err "$str"
        FAIL=1
    fi

    export ELOS_CONFIG_PATH=${REAL_ELOS_CONFIG_PATH-"$SMOKETEST_DIR/config.json"}
    return $FAIL
}

smoketest_client_uds() {
    prepare_env "client_uds"

    MESSAGE="{\"messageCode\": 1001, \"severity\": 4, \"payload\": \"a custom event with severity INFO (4)\"}"

    RESULT=0
    LOG_ELOSD="$RESULT_DIR/elosd.log"
    REAL_ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH}
    export ELOS_CONFIG_PATH="${RESULT_DIR}/test_config.json"
    cp "${REAL_ELOS_CONFIG_PATH}" "${ELOS_CONFIG_PATH}"

    log "Starting elosd"
    elosd > "$LOG_ELOSD" 2>&1 &
    ELOSD_PID=$!

    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    log "Publish Message via unix socket"
    elosc -U "$ELOSD_SOCKET_PATH" -p "$MESSAGE" >> "$RESULT_DIR/elosc_unix_publish.txt" 2>&1

    log "Find message published via unix socket"
    elosc -U "$ELOSD_SOCKET_PATH" -f ".event.messageCode 1001 EQ" > "$RESULT_DIR/elosc_unix_publish_event.log" 2>&1

    if grep -q "\"messageCode\":1001" "$RESULT_DIR/elosc_unix_publish_event.log"; then
        log "Connection established, message logged as expected"
    else
        log_err "Message not published via unix socket"
        RESULT=1
    fi
    
    log "Stop elosd ($ELOSD_PID) ..."
    kill "$ELOSD_PID" > /dev/null
    wait "$ELOSD_PID" > /dev/null
    log "done"

    export ELOS_CONFIG_PATH="${REAL_ELOS_CONFIG_PATH}"

    return "$RESULT"

}

smoketest_coredump() {
    prepare_env "coredump"

    RESULT=0
    LOG_ELOSD="$RESULT_DIR/elosd.log"


    REAL_COREDUMP_CONFIG_FILE=$ELOS_COREDUMP_CONFIG_FILE
    export ELOS_COREDUMP_CONFIG_FILE=${RESULT_DIR}/test_config_coredump.json

    REAL_ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH}
    export ELOS_CONFIG_PATH=${RESULT_DIR}/test_config.json
    cp "${REAL_ELOS_CONFIG_PATH}" "${ELOS_CONFIG_PATH}"

    ELOS_DLT_PIPE_PATH="${RESULT_DIR}/dlt"
    sed -i "s,/tmp/dlt,${ELOS_DLT_PIPE_PATH}," "${ELOS_CONFIG_PATH}"
    start_dlt_mock

    log "Starting elosd"
    elosd > $LOG_ELOSD 2>&1 &
    ELOSD_PID=$!

    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    log "Starting elos coredump test"

    SOCKETS="TCP UNIX"
    for SOCKET in $SOCKETS; do
        log "Using socket: $SOCKET"

        if [ "$SOCKET" = "TCP" ]; then
            sed "s#\"networkAddress\": *\"[^\"]*\"#\"networkAddress\": \"127.0.0.1:$ELOSD_PORT\"#" "$REAL_COREDUMP_CONFIG_FILE" > "$ELOS_COREDUMP_CONFIG_FILE"
            echo $TEST_MESSAGE | elos-coredump 1 /usr/bin/example 2 3 11 333333 exampletest > $RESULT_DIR/coredump_trigger_$SOCKET.log 2>&1
            elosc -P $ELOSD_PORT -f ".event.messageCode 5100 EQ" > $RESULT_DIR/coredump_event_$SOCKET.log 2>&1
        fi

        if [ "$SOCKET" = "UNIX" ]; then
            sed "s#\"networkAddress\": *\"[^\"]*\"#\"networkAddress\": \"${SMOKETEST_TMP_DIR}/elosd.socket\"#" "$REAL_COREDUMP_CONFIG_FILE" > "$ELOS_COREDUMP_CONFIG_FILE"
            echo $TEST_MESSAGE | elos-coredump 1 /usr/bin/example 2 3 11 333333 exampletest > $RESULT_DIR/coredump_trigger_$SOCKET.log 2>&1
            elosc -U ${SMOKETEST_TMP_DIR}/elosd.socket -f ".event.messageCode 5100 EQ" > $RESULT_DIR/coredump_event_$SOCKET.log 2>&1
        fi

        if grep -q "\"messageCode\":5100" $RESULT_DIR/coredump_event_$SOCKET.log; then
            log "Success: coredump event logged for socket $SOCKET"
        else
            log_err "Error: coredump event not logged for socket $SOCKET"
            RESULT=1
        fi
    done

    log "Stop elosd ($ELOSD_PID) ..."
    kill $ELOSD_PID > /dev/null
    wait $ELOSD_PID > /dev/null
    stop_dlt_mock
    log "done"


    export ELOS_CONFIG_PATH="${REAL_ELOS_CONFIG_PATH}"
    export ELOS_COREDUMP_CONFIG_FILE="${REAL_COREDUMP_CONFIG_FILE}"

    return $RESULT
}

smoketest_syslog() {
    prepare_env "syslog"

    TEST_MESSAGE="an arbitrary syslog message"
    LOG_ELOSD="$RESULT_DIR/elosd.log"

    log "Starting elosd"
    elosd > $LOG_ELOSD 2>&1 &
    ELOSD_PID=$!

    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_file $ELOS_SYSLOG_PATH
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    log "Starting syslog test"
    syslog_example -m "$TEST_MESSAGE" -P $ELOSD_PORT > $RESULT_DIR/syslog_example.log 2>&1 &
    SYSLOG_EXAMPLE_PID=$!

    log "wait for syslog_example to finish ..."
    wait $SYSLOG_EXAMPLE_PID
    log "done"

    log "Stop elosd ($ELOSD_PID) ..."
    kill $ELOSD_PID > /dev/null
    wait $ELOSD_PID > /dev/null
    log "done"

    TEST_RESULT=0
    grep "\[receive message\] " $RESULT_DIR/syslog_example.log | grep -q "$TEST_MESSAGE"
    if [ $? -ne 0 ]; then
        log_err "missing message: '$TEST_MESSAGE'"
        TEST_RESULT=1
    fi

    return $TEST_RESULT
}

elosd_built_with_libsystemd() {
    # Check if elosd is linked against libsystemd. In minimal systems, the ldd command may not be available, so fall
    # back to simply checking whether this library is mentioned in the binary.
    if which ldd > /dev/null 2>&1; then
        ldd "$(which elosd)" | grep -q libsystemd.so
        return $?
    else
        strings "$(which elosd)" | grep -q libsystemd.so
        return $?
    fi
}

smoketest_syslog_systemd() {
    prepare_env "syslog_systemd"

    TEST_MESSAGE="an arbitrary syslog message"
    LOG_ELOSD="$RESULT_DIR/elosd.log"

    if ! elosd_built_with_libsystemd; then
        log "elosd built with systemd support disabled, skipping test."
        return 0
    fi

    log "Starting elosd"
    # Open the syslog socket; when the first client sends data, start elosd.
    # systemd-socket-activate does not pass through most environment variables, so explicitly instruct it to pass
    # all elos-related env and the LD_LIBRARY_PATH.
    # If elosd did not correctly use the socket passed by systemd-socket-activate and re-opened the socket path on its own instead,
    # the test might wrongly succeed. So explicitly set the ELOS_SYSLOG_PATH to a different (invalid) path to prevent this.

    # word splitting of the env command is intentional, each env entry is intended to be passed as a separate argument, with the leading "-E" also being a separate arg.
    # shellcheck disable=SC2046
    systemd-socket-activate -d -l "$ELOS_SYSLOG_PATH" -E ELOS_SYSLOG_USE_SYSTEMD_SOCKET=1 -E LD_LIBRARY_PATH $(env | grep ^ELOS | awk '{ print "-E"; print $1 }') \
        -E ELOS_SYSLOG_PATH=/invalid/path \
        elosd > "$LOG_ELOSD" 2>&1 &
    ELOSD_PID=$!
    log "Starting syslog socket activation test"

    wait_for_file "$ELOS_SYSLOG_PATH"
    # let systemd-socket-activate start elosd by writing to the syslog socket
    echo "" | socat "unix-sendto:${ELOS_SYSLOG_PATH}" -
    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    syslog_example -m "$TEST_MESSAGE" -P "$ELOSD_PORT" > "$RESULT_DIR/syslog_example.log" 2>&1 &
    SYSLOG_EXAMPLE_PID=$!

    log "wait for syslog_example to finish ..."
    wait "$SYSLOG_EXAMPLE_PID"
    log "done"

    log "Stop elosd ($ELOSD_PID) ..."
    kill "$ELOSD_PID" > /dev/null
    wait "$ELOSD_PID" > /dev/null
    log "done"

    TEST_RESULT=0
    if ! grep "\[receive message\] " "$RESULT_DIR/syslog_example.log" | grep -q "$TEST_MESSAGE"; then
        log_err "missing message: '$TEST_MESSAGE'"
        TEST_RESULT=1
    fi

    return "$TEST_RESULT"
}

smoketest_kmsg() {
    prepare_env "kmsg"

    LOG_ELOSD="$RESULT_DIR/elosd.log"
    LOG_ELOSCL="$RESULT_DIR/elosc_poll.log"
    LOG_ELOSC_FIND="$RESULT_DIR/elosc_find.log"
    LOG_KMSG="$RESULT_DIR/kmsg_example.log"
    TEST_MESSAGE="12,1234,12345678901,-;smoketest: kernel message"
    FILTERSTRING=".event.source.fileName '${ELOS_KMSG_FILE}' STRCMP"

    log "Starting elosd"
    elosd > $LOG_ELOSD 2>&1 &
    ELOSD_PID=$!

    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_file $ELOS_KMSG_FILE
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    log "Polling KMSG"
    elosc -P $ELOSD_PORT -s "$FILTERSTRING" > $LOG_ELOSCL 2>&1 &
    POLL_CLIENT_PID=$!
    sleep 1s

    log "Writing kernel message"
    echo $TEST_MESSAGE > $ELOS_KMSG_FILE 2> $LOG_KMSG
    sleep 1s

    elosc -P $ELOSD_PORT -f "$FILTERSTRING" > $LOG_ELOSC_FIND 2>&1 

    log "Stop elosd ($ELOSD_PID)"

    kill $ELOSD_PID > /dev/null
    wait $ELOSD_PID > /dev/null

    log "Stop elosc ($POLL_CLIENT_PID)"
    kill $POLL_CLIENT_PID > /dev/null
    wait $POLL_CLIENT_PID > /dev/null

    TEST_RESULT=0
    if ! message_count=$(grep -c "${TEST_MESSAGE}" "${LOG_ELOSCL}") \
        || [ $message_count -ne 1 ]; then
            log_err "unexpected message count (${message_count}) published: '$TEST_MESSAGE'"
        TEST_RESULT=1
    fi

    if ! message_count=$(grep -c "${TEST_MESSAGE}" "${LOG_ELOSC_FIND}") \
        || [ $message_count -ne 1 ]; then
            log_err "unexpected message count (${message_count}) in log storage: '$TEST_MESSAGE'"
        TEST_RESULT=1
    fi

    return $TEST_RESULT
}

check_for_attribute() {
    local KEY=$1
    local VALUE=$2

    grep "\"$KEY\":$VALUE" $RESULT_DIR/elosc_poll_1.log > /dev/null &
    grep "\"$KEY\":$VALUE" $RESULT_DIR/elosc_poll_2.log > /dev/null
    if [ $? -ne 0 ]; then
        log_err "missing '$KEY': '$VALUE'"
        return 1
    fi
    return 0
}

smoketest_publish_poll() {
    prepare_env "publish_poll"

    LOG_ELOSD="$RESULT_DIR/elosd.log"
    local ELOSC_FILE_NAME=$(which elosc)
    local FILTERSTRING=".event.source.appName 'publish_poll' STRCMP"
    local MESSAGE_TEMPLATE="
{
  \"date\": [%s],
  \"source\": {
    \"appName\": \"publish_poll\",
    \"fileName\": \"$ELOSC_FILE_NAME\",
    \"pid\": 42
  },
  \"severity\": %d,
  \"hardwareid\": \"$HOSTNAME\",
  \"classification\": $(printf %u 0x0000BEEFCAFFEE00),
  \"messageCode\": %d,
  \"payload\": \"test message %d\"
}"

    log "Starting elosd"
    elosd > "${LOG_ELOSD}" 2>&1 &
    ELOSD_PID=$!
    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    log "Polling client 1 ..."
    elosc -P $ELOSD_PORT -s "$FILTERSTRING" > $RESULT_DIR/elosc_poll_1.log 2>&1 &
    POLL_CLIENT_1_PID=$!

    log "Polling client 2 ..."
    elosc -P $ELOSD_PORT -s "$FILTERSTRING" > $RESULT_DIR/elosc_poll_2.log 2>&1 &
    POLL_CLIENT_2_PID=$!

    sleep 0.5s

    for i in `seq 1 10`; do
        local MESSAGE=$(printf "$MESSAGE_TEMPLATE" `date "+%s,0"` $i $i $i )
        log "Publish \"$MESSAGE\""
        elosc -P $ELOSD_PORT -p "$MESSAGE" >> $RESULT_DIR/elosc_publish.log 2>&1
    done

    sleep 1s
    kill $POLL_CLIENT_1_PID $POLL_CLIENT_2_PID $ELOSD_PID >/dev/null
    wait $POLL_CLIENT_1_PID $POLL_CLIENT_2_PID $ELOSD_PID > /dev/null
    log "done ($?)"

    local TEST_RESULT=0
    local ELOSC_FILE_NAME_ESCAPED=$(echo $ELOSC_FILE_NAME | sed 's@/@\\\\\/@g')
    for i in `seq 1 10`; do
        check_for_attribute "payload" "\"test message $i\"" \
        && check_for_attribute "messageCode" "$i" \
        && check_for_attribute "date" "" \
        && check_for_attribute "appName" "\"publish_poll\"" \
        && check_for_attribute "pid" "42" \
        && check_for_attribute "severity" "$i" \
        && check_for_attribute "hardwareid" "\"$HOSTNAME\"" \
        && check_for_attribute "classification" "$(printf %u 0x0000BEEFCAFFEE00)" \
        && check_for_attribute "fileName" "\"${ELOSC_FILE_NAME_ESCAPED}\"" \
        || TEST_RESULT=1
    done

    return $TEST_RESULT
}

smoketest_locale() {
    prepare_env "locale"

    local VALID_JSON_MESSAGE="{\"severity\":1,\"hardwareid\":\"localhost\",\"classification\":42.5,\
\"messageCode\":32,\"payload\":\"this_is_payload\"}"

    local INVALID_JSON_MESSAGE="{\"date\":[%s],\"source\":{\"appName\":\"☃\",\
\"fileName\":\"𝔾𝓻ÿ𝓣𝔃ë𝐋𝐵𝓲𝓂𝓕\",\"pid\":42},\"severity\":💀💀💀,\"hardwareid\":\"🙊\",\
\"classification\":\"🙉\",\"messageCode\":\"🙈\",\"payload\":\"♔♕♖♗♘♙♚♛♜♝♞♟\"}"

    LOG_ELOSD="$RESULT_DIR/elosd.log"

#start elos and client

    log "Starting elosd"
    elosd > "${LOG_ELOSD}" 2>&1 &
    ELOSD_PID=$!
    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    sleep 0.5s

    log "Start listening client"
    elosc -P $ELOSD_PORT -s "1 1 EQ" -r 100 >> $RESULT_DIR/event.log 2>&1 &
    CLIENT_PID=$!


    #send valid messages
    tinyElosc -P $ELOSD_PORT -v >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -s ".event.payload 'Ϡ𝔾𝓻ÿ𝓣𝔃ë𝐋Ϡ' STRCMP" >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -p "{\"payload\":\"this is payload\"}" >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -p "{\"payload\":\"Ϡ𝔾𝓻ÿ𝓣𝔃ë𝐋Ϡ\"}" >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -p $VALID_JSON_MESSAGE >> $RESULT_DIR/event.log 2>&1

    #send invalid messages
    tinyElosc -P $ELOSD_PORT -s "\"Ϡ𝔾𝓻ÿ𝓣𝔃ë𝐋Ϡ\"" >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -p "Ϡ𝔾𝓻ÿ𝓣𝔃ë𝐋Ϡ" >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -p "{\"Ϡ𝔾𝓻ÿ𝓣𝔃ë𝐋Ϡ\"}" >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -p "{\"invalid\":Ϡ𝔾𝓻ÿ𝓣𝔃ë𝐋Ϡ}" >> $RESULT_DIR/event.log 2>&1
    tinyElosc -P $ELOSD_PORT -p $INVALID_JSON_MESSAGE >> $RESULT_DIR/event.log 2>&1

    #send empty messages
    tinyElosc -P $ELOSD_PORT -s "" >> $RESULT_DIR/event.log 2>&1 #invalid
    tinyElosc -P $ELOSD_PORT -p "" >> $RESULT_DIR/event.log 2>&1 #invalid

    #check if elosd is still alive
    local alive=0
    local success=1
    log $(ps -p ${ELOSD_PID})
    if [ $? -eq 0 ]
    then
        alive=$((alive+1))
        success=$((success+1))
    fi

    #restart elosd and client with different locale
    kill $CLIENT_PID $ELOSD_PID >/dev/null
    wait $CLIENT_PID $ELOSD_PID > /dev/null

    log "Change locale to DE"
    export LC_ALL=de_DE.utf8

    log "Restart elosd and client"
    elosd > "${LOG_ELOSD}" 2>&1 &
    ELOSD_PID=$!
    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    elosc -P $ELOSD_PORT -s "1 1 EQ" -r 100 >> $RESULT_DIR/event.log 2>&1 &
    CLIENT_PID=$!
    sleep 0.5s

    #locale tests
    tinyElosc -P $ELOSD_PORT -p "{\"classification\":42,5}" >> $RESULT_DIR/event.log 2>&1 #invalid
    tinyElosc -P $ELOSD_PORT -p "{\"payload\":\"42,5\"}" >> $RESULT_DIR/event.log 2>&1 #valid
    tinyElosc -P $ELOSD_PORT -p "{\"payload\":\"$(date)\"}" >> $RESULT_DIR/event.log 2>&1 #valid
    sleep 0.1s

    #check test success
    log $(ps -p ${ELOSD_PID})
    if [ $? -eq 0 ]
    then
        alive=$((alive+1))
        success=$((success+1))
    fi

    kill $CLIENT_PID >/dev/null
    wait $CLIENT_PID > /dev/null
    kill $ELOSD_PID >/dev/null
    wait $ELOSD_PID > /dev/null

    sync $RESULT_DIR

    local versionResponses=$(cat $RESULT_DIR/event.log | grep -wc "\"version\":")
    log "found $versionResponses valid version messages and expected 1"
    if [ $versionResponses -eq 1 ]; then
        success=$((success+1))
    fi
    local nonErrors=$(cat $RESULT_DIR/event.log | grep -wc "\"error\":null")
    #note: "tinyElosc -P $ELOSD_PORT -s" always sends an additional valid eventCreate that is counted too
    log "found $nonErrors valid messages and expected 7"
    if [ $nonErrors -eq 7 ]; then
        success=$((success+1))
    fi

    local errors1=$(cat $RESULT_DIR/event.log | grep -wc error)
    local errors2=$(cat $RESULT_DIR/event.log | grep -wc ERR)
    local errorCount=$((errors1 + errors2 - nonErrors))
    log "found $errorCount errors and expected 8"
    if [ $errorCount -eq 8 ]; then
        success=$((success+1))
    fi

    if [ $alive -eq 2 ]; then
        log "elosd kept running after all invalid messages"
    else
        log "elosd was killed by the messages: $alive"
    fi

    if [ $success -eq 6 ]; then
        return 0
    else
        return $((6-success))
    fi
}

smoketest_find_event() {
    prepare_env "find_event"
    set +e

    local LOG_ELOSD="$RESULT_DIR/elosd.log"
    local LOG_ELOSC_PUBLISH="$RESULT_DIR/elosc_publish.log"
    local LOG_ELOSC_SUBSCRIBE="$RESULT_DIR/elosc_subscribe.log"
    local LOG_ELOSC_UNSUBSCRIBE="$RESULT_DIR/elosc_unsubscribe.log"
    local LOG_ELOSC_FINDEVENT="$RESULT_DIR/elosc_findevent.log"

    local MESSAGE01="{\"messageCode\":4,\"payload\":\"testEventFiltering\"}"
    local MESSAGE02="{\"messageCode\":40,\"payload\":\"testEventFiltering\"}"
    local MESSAGE03="{\"messageCode\":400,\"payload\":\"testEventFiltering\"}"
    local FILTERSTRING=".event.messageCode 400 EQ"

    local ELOSD_PID
    local ELOSD_ALIVE=0
    local ELOSC_FINDEVENT_MATCHES=0
    local ELOSC_SUBSCRIBE_PID

    # Setup environment
    log "Start elosd..."
    ELOS_STORAGE_BACKEND_JSONBACKEND_FILE="$RESULT_DIR/elosd_event_%count%.log" elosd > "${LOG_ELOSD}" 2>&1 &
    ELOSD_PID=$!
    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    log "Start subscriber client ..."
    elosc -P $ELOSD_PORT -s "$FILTERSTRING" -r 100 > $LOG_ELOSC_SUBSCRIBE 2>&1 &
    ELOSC_SUBSCRIBE_PID=$!
    sleep 0.5s

    # Fetch the corresponding queue id
    EVENT_QUEUE_ID=$(sed -n "s/successfully subscribed to event queue \([0-9]\+\)/\1/p" "$LOG_ELOSC_SUBSCRIBE")
    log "Got event queue id $EVENT_QUEUE_ID"

    # Publish messages
    elosc -P $ELOSD_PORT -p "$MESSAGE01" > $LOG_ELOSC_PUBLISH 2>&1
    elosc -P $ELOSD_PORT -p "$MESSAGE02" >> $LOG_ELOSC_PUBLISH 2>&1
    elosc -P $ELOSD_PORT -p "$MESSAGE03" >> $LOG_ELOSC_PUBLISH 2>&1
    sleep 0.5s

    # Search in the log for specific messages
    log "Ask elosd to find matching events..."
    elosc -P $ELOSD_PORT -f "$FILTERSTRING" > $LOG_ELOSC_FINDEVENT 2>&1

    # Unsubscribe from event queues
    elosc -P $ELOSD_PORT -u "$EVENT_QUEUE_ID" > $LOG_ELOSC_UNSUBSCRIBE 2>&1

    # Check success conditions
    ELOSC_FINDEVENT_MATCHES=$(grep -wc testEventFiltering $LOG_ELOSC_FINDEVENT)
    log $(ps -p ${ELOSD_PID})
    if ps -p ${ELOSD_PID} 2>&1 >/dev/null; then
        ELOSD_ALIVE=1
    fi

    #teardown
    log "$(kill -TERM $ELOSC_SUBSCRIBE_PID $ELOSD_PID 2>&1)"
    log "$(wait $ELOSC_SUBSCRIBE_PID $ELOSD_PID 2>&1)"

    if [ "$ELOSD_ALIVE" = "1" ] && [ "$ELOSC_FINDEVENT_MATCHES" = "1" ]
    then
        log "TEST SUCCESS"
        return 0
    elif [ "$ELOSD_ALIVE" != "1" ]
    then
        log "TEST FAILED since elosd stopped working"
        return 1
    else
        log "TEST FAILED with $ELOSC_FINDEVENT_MATCHES matching events while 1 was expected"
        return 2
    fi
}

smoketest_plugins() {
    prepare_env "plugins"

    LOG_ELOSD="$RESULT_DIR/elosd.log"
    TEST_RESULT=0

    REAL_ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH}
    export ELOS_CONFIG_PATH=${RESULT_DIR}/test_config.json
    cp "${REAL_ELOS_CONFIG_PATH}" "${ELOS_CONFIG_PATH}"

    ELOS_DLT_PIPE_PATH="${RESULT_DIR}/dlt"
    sed -i "s,/tmp/dlt,${ELOS_DLT_PIPE_PATH}," "${ELOS_CONFIG_PATH}"

    start_dlt_mock

    log "Starting Elosd with config ${ELOS_CONFIG_PATH}"
    elosd > "$LOG_ELOSD" 2>&1 &
    ELOSD_PID=$!
    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_elosd_claims_running "${LOG_ELOSD}"
    log "Stop elosd ($ELOSD_PID)"
    kill $ELOSD_PID #> /dev/null
    wait $ELOSD_PID #> /dev/null

    stop_dlt_mock

    export ELOS_CONFIG_PATH="${REAL_ELOS_CONFIG_PATH}"

    PLUGINS="Dummy ScannerDummy"
    if [ -n "$(find "$ELOS_BACKEND_PATH" -name backend_dlt_logger.so)" ]; then
        PLUGINS="$PLUGINS DLT"
    else
        log "DLT plugin not installed, not checking for it."
    fi

    for plugin in ${PLUGINS}; do
        TEST_MATCH="/Plugin\s.${plugin}/!d; /loaded/p; /started/p; /Stopping/p; /Unloading/p;"
        TEST_COUNT=$(sed -n -e "$TEST_MATCH" "$LOG_ELOSD" | wc -l)
        if [ "$TEST_COUNT" != "4" ]; then
            log_err "Missing messages for Plugin '${plugin}' ($TEST_COUNT of 4 lines found)"
            TEST_RESULT=1
        fi
    done

    return $TEST_RESULT
}

smoketest_dual_json_plugin() {
    prepare_env "dual_json_plugin"

    local LOG_ELOSD="$RESULT_DIR/elosd.log"

    export ELOS_CONFIG_PATH=$SMOKETEST_DIR/config_dual.json
    export ELOS_STORAGE_BACKEND_COREDUMP_FILE=$RESULT_DIR/elos_coredump_%count%.log
    COREDUMP_FILE=$RESULT_DIR/elos_coredump_0.log
    export ELOS_STORAGE_BACKEND_JSONBACKEND_FILE=$RESULT_DIR/elos_jsonbackend_%count%.log
    JSONBACKEND_FILE=$RESULT_DIR/elos_jsonbackend_0.log
    TEST_RESULT=0

    log "Starting elosd"
    elosd > "$LOG_ELOSD" 2>&1 &
    ELOSD_PID=$!

    wait_for_elosd_socket "${ELOSD_PID}"
    wait_for_file $COREDUMP_FILE
    wait_for_file $JSONBACKEND_FILE
    wait_for_elosd_claims_running "${LOG_ELOSD}"

    elosc -P $ELOSD_PORT -p "{\"payload\":\"coredump\", \"messageCode\":5005}" >> $RESULT_DIR/event.log 2>&1
    elosc -P $ELOSD_PORT -p "{\"payload\":\"not coredump\", \"messageCode\":5004}" >> $RESULT_DIR/event.log 2>&1

    log "Stop elosd ($ELOSD_PID)"
    kill $ELOSD_PID > /dev/null
    wait $ELOSD_PID > /dev/null

    coredump_number=$(wc -l $COREDUMP_FILE | sed "s# $COREDUMP_FILE##g" )
    if [ "$coredump_number" != "1" ]; then
        log_err "unexpected amount of coredumps: $coredump_number"
        TEST_RESULT=1
    fi

    jsonbackend_number=$(wc -l $JSONBACKEND_FILE | sed "s# $JSONBACKEND_FILE##g" )
    if [ "$jsonbackend_number" != "1" ]; then
        log_err "unexpected amount of regular logs: $jsonbackend_number"
        TEST_RESULT=1
    fi

    return $TEST_RESULT
}

smoketest_compile_program_using_libelos() {
    prepare_env "compile_program_using_libelos"
    TEST_RESULT=0

    EXTRA_FLAGS=""
    if run_in_source_tree; then
        BUILD_DEPS_PREFIX="${SMOKETEST_DIR}/../../build/deps"
        EXTRA_FLAGS="-I ${BUILD_DEPS_PREFIX}/include/ -L ${BUILD_DEPS_PREFIX}/lib"
    fi

    log "Try to compile simple program using libelos"
    printf '#include <elos/libelos/libelos.h>\nint main(int argc, char* argv[]){return 0;}' \
        | gcc -v -xc -lelos -lelos_common \
        -I "${PREFIX_PATH}/include/" -L "${PREFIX_PATH}/lib" \
        ${EXTRA_FLAGS} \
        -o "${SMOKETEST_TMP_DIR}/testlibelos" - \
        >> "$RESULT_DIR/libelos.log" 2>&1
    if [ $? -ne 0 ]; then
        log_err "failed to compile test program for libelos"
        TEST_RESULT=1
    fi

    if run_in_source_tree; then
        log "Try to compile syslog demo using libelos"
        gcc -v  \
            -I "${PREFIX_PATH}/include/" -L "${PREFIX_PATH}/lib" \
            ${EXTRA_FLAGS} \
            -o "${SMOKETEST_TMP_DIR}/testlibelos_syslog" "${SMOKETEST_DIR}/../../src/demos/syslog.c" \
            -lelos -lsafu -lelos_common \
            >> "$RESULT_DIR/libelos.log" 2>&1
        if [ $? -ne 0 ]; then
            log_err "failed to compile test program for libelos"
            TEST_RESULT=1
        fi
    fi

    return $TEST_RESULT
}

smoketest_compile_program_using_libeloscpp() {
    prepare_env "compile_program_using_libeloscpp"
    TEST_RESULT=0

    EXTRA_FLAGS=""
    if run_in_source_tree; then
        BUILD_DEPS_PREFIX="${SMOKETEST_DIR}/../../build/deps"
        EXTRA_FLAGS="-I ${BUILD_DEPS_PREFIX}/include/ -L ${BUILD_DEPS_PREFIX}/lib"
    fi

    log "Try to compile simple program using libelos-cpp"
    printf '#include <elos/libelos-cpp/libelos-cpp.h>\nint main(int argc, char* argv[]){return 0;}' \
        | g++ -v -xc++ -std=c++14 \
        -I "${PREFIX_PATH}/include/" -L "${PREFIX_PATH}/lib" \
        ${EXTRA_FLAGS} \
        -o "${SMOKETEST_TMP_DIR}/testlibelos-cpp" - -lelos-cpp\
        >> "$RESULT_DIR/libelos-cpp.log" 2>&1
    if [ $? -ne 0 ]; then
        log_err "failed to compile test program for libelos-cpp"
        TEST_RESULT=1
    fi

    return $TEST_RESULT
}

smoketest_compile_program_with_cpp() {
    prepare_env "compile_program_with_cpp"
    TEST_RESULT=0

    EXTRA_FLAGS=""
    if run_in_source_tree; then
        BUILD_DEPS_PREFIX="${SMOKETEST_DIR}/../../build/deps"
        EXTRA_FLAGS="-I ${BUILD_DEPS_PREFIX}/include/ -L ${BUILD_DEPS_PREFIX}/lib"
    fi

    log "Try to compile a simple C++ program using libelos"
    printf "%s\n" \
        '#include <safu/flags.h>' \
        '#include <elos/libelos/libelos.h>' \
        '#include <safu/ringbuffer.h>' \
        '' \
        'safuResultE_t deleteFunc(void *) { '\
        '    puts("deleteFunc got called correctly");' \
        '    return SAFU_RESULT_OK;' \
        '}' \
        '' \
        'int main(int argc, char* argv[]) {' \
        '    safuResultE_t result;'\
        '    safuRingBuffer_t rbuf = {};' \
        '    safuRingBufferParam_t param = {4, true, &deleteFunc};' \
        '    uint32_t element = 0;' \
        '    result = safuRingBufferInitialize(&rbuf, &param);' \
        '    if (result != SAFU_RESULT_OK) { puts("init fail"); return 100; }' \
        '    result = safuRingBufferWrite(&rbuf, &element);' \
        '    if (result != SAFU_RESULT_OK) { puts("wr fail"); return 101; }' \
        '    result = safuRingBufferDeleteMembers(&rbuf);' \
        '    if (result != SAFU_RESULT_OK) { puts("del fail"); return 102; }' \
        '    return 0;' \
        '}' \
        | g++ -v -xc++ -std=c++11 \
        -I "${PREFIX_PATH}/include/" -L "${PREFIX_PATH}/lib" \
        ${EXTRA_FLAGS} \
        -o "${SMOKETEST_TMP_DIR}/testlibelos" - -lelos -lsafu \
        >> "$RESULT_DIR/cpp_compile.log" 2>&1
    if [ $? -ne 0 ]; then
        log_err "failed to compile C++ test program"
        TEST_RESULT=1
    fi

    if [ "$BUILD_TYPE" = "Release" ]; then
        ${SMOKETEST_TMP_DIR}/testlibelos \
            > "$RESULT_DIR/cpp_compile_test.log" 2>&1
        retval=$?
        if [ $retval -ne 0 ]; then
            log_err "failed to run C++ test program (returned $retval)"
            TEST_RESULT=1
        else
            testval=$(grep -c "deleteFunc got called correctly" "$RESULT_DIR/cpp_compile_test.log")
            if [ "$testval" != "1" ]; then
                log_err "unexpected C++ test program output"
                TEST_RESULT=1
            fi
        fi
    fi

    return $TEST_RESULT
}

smoketest_compile_program_using_pkgconfig() {
    prepare_env "compile_program_using_pkgconfig"
    TEST_RESULT=0
    PKGCONFIG_PARAM="--print-errors --errors-to-stdout"

    # Compile program using libelos
    log "Query libelos version with pkg-config"
    pkgconfig_version=$(pkg-config elos --modversion ${PKGCONFIG_PARAM})
    if [ $? -ne 0 ]; then TEST_RESULT=1; fi
    pkgconfig_cflags=$(pkg-config elos --cflags ${PKGCONFIG_PARAM})
    if [ $? -ne 0 ]; then TEST_RESULT=1; fi
    pkgconfig_libs=$(pkg-config elos --libs ${PKGCONFIG_PARAM})
    if [ $? -ne 0 ]; then TEST_RESULT=1; fi
    if [ $TEST_RESULT -ne 0 ]; then
        log_err "failed to query pkg-config data for libelos"
        log_err "output for 'pkg-config elos --modversion'"
        log_err "${pkgconfig_version}"
        log_err "output for 'pkg-config elos --cflags'"
        log_err "${pkgconfig_cflags}"
        log_err "output for 'pkg-config elos --libs'"
        log_err "${pkgconfig_libs}"
        TEST_RESULT=1
    fi

    log "Try to compile simple program using libelos with pkg-config"
    printf '#include <elos/libelos/libelos.h>\nint main(int argc, char* argv[]){return 0;}' \
        | gcc -v -xc \
        ${pkgconfig_cflags} \
        -o "${SMOKETEST_TMP_DIR}/testlibelos " - \
        ${pkgconfig_libs} \
        >> "$RESULT_DIR/libelos.log" 2>&1
    if [ $? -ne 0 ]; then
        log_err "failed to compile test program for libelos"
        TEST_RESULT=1
    fi

    # Compile program using libelosplugin
    log "Query libelosplugin version with pkg-config"
    pkgconfig_version=$(pkg-config elosplugin --modversion ${PKGCONFIG_PARAM})
    if [ $? -ne 0 ]; then TEST_RESULT=1; fi
    pkgconfig_cflags=$(pkg-config elosplugin --cflags ${PKGCONFIG_PARAM})
    if [ $? -ne 0 ]; then TEST_RESULT=1; fi
    pkgconfig_libs=$(pkg-config elosplugin --libs ${PKGCONFIG_PARAM})
    if [ $? -ne 0 ]; then TEST_RESULT=1; fi
    if [ $TEST_RESULT -ne 0 ]; then
        log_err "failed to query pkg-config data for libelosplugin"
        log_err "output for 'pkg-config elosplugin --modversion'"
        log_err "${pkgconfig_version}"
        log_err "output for 'pkg-config elosplugin --cflags'"
        log_err "${pkgconfig_cflags}"
        log_err "output for 'pkg-config elosplugin --libs'"
        log_err "${pkgconfig_libs}"
        TEST_RESULT=1
    fi

    log "Try to compile simple program using libelosplugin with pkg-config"
    printf '#include <elos/libelosplugin/libelosplugin.h>\nint main(int argc, char* argv[]){return 0;}' \
        | gcc -v -xc \
        ${pkgconfig_cflags} \
        -o "${SMOKETEST_TMP_DIR}/testlibelosplugin " - \
        ${pkgconfig_libs} \
        >> "$RESULT_DIR/libelosplugin.log" 2>&1
    if [ $? -ne 0 ]; then
        log_err "failed to compile test program for libelosplugin"
        TEST_RESULT=1
    fi

    return $TEST_RESULT
}

# $1 - test name
# $2 - (optional) test function - valid options are [test_expect_success|test_expect_failure|test_expect_unstable]
call_test() {
    test_name=$1
    test_method=${2-"test_expect_success"}


    local result=1
    local skipped="false"

    start_time=$(date +%s)
    echo -n "${test_name} ... "

    if [ "$ENABLED_TESTS" = "" ]; then
        echo $DISABLED_TESTS | grep -q "$test_name\b"
        if [ $? -ne 0 ]; then
            smoketest_${test_name}
            result=$?
        else
            skipped="true"
        fi
    else
        echo $ENABLED_TESTS | grep -q  "$test_name\b"
        if [ $? -eq 0 ]; then
                smoketest_${test_name}
                result=$?
        else
            skipped="true"
        fi
    fi

    test_status="FAILED"
    if [ "${skipped}" = "true" ]; then
        test_status="SKIPPED"
        result=0
    else
        if [ ${result} -eq 0 ]; then
            test_status="OK"
        fi
    fi

    echo "${test_status}"
    end_time=$(date +%s)
    printf '%s %s %s' "${test_name}" "$((end_time - start_time))" "${test_status}" > "${RESULT_DIR}/smoketest.result" 

    return ${result}
}


while [ $# -gt 0 ]; do
    case ${1} in
        --help|-h)
            print_info
            exit 0;;
        *)
            echo "error: unknown option: $1"
            echo "Use ${SCRIPT_NAME} --help"
            exit 1 ;;
    esac
    shift
done

FAILED_TESTS=0
call_test "elosd" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "elosd_config_not_found" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "client_uds" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "syslog" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "syslog_systemd" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "coredump" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "kmsg" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "publish_poll" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "locale" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "find_event" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "plugins" || FAILED_TESTS=$((FAILED_TESTS+1))
call_test "dual_json_plugin" || FAILED_TESTS=$((FAILED_TESTS+1))

if [ "${SMOKETEST_ENABLE_COMPILE_TESTS}" != "" ]; then
    call_test "compile_program_using_libelos" || FAILED_TESTS=$((FAILED_TESTS+1))
    call_test "compile_program_using_libeloscpp" || FAILED_TESTS=$((FAILED_TESTS+1))
    call_test "compile_program_with_cpp" || FAILED_TESTS=$((FAILED_TESTS+1))
    call_test "compile_program_using_pkgconfig" || FAILED_TESTS=$((FAILED_TESTS+1))
fi

create_junit_report "${SMOKETEST_RESULT_DIR}"

exit ${FAILED_TESTS}
