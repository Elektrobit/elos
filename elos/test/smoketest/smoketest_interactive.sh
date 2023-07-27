CMDPATH=$(realpath "$(dirname "$0")")
BASE_DIR=$(realpath "$CMDPATH/../..")
BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE/"
DIST_DIR="${BUILD_DIR}/dist"

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH-""}:"${DIST_DIR}/usr/local/lib"
export PATH=${PATH}:"${DIST_DIR}/usr/local/bin"
export SMOKETEST_DIR=${SMOKETEST_DIR-"$(dirname "$0")"}
export SMOKETEST_RESULT_DIR=${SMOKETEST_RESULT_DIR-"$BUILD_DIR/result/smoketest_results"}
export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR-"/tmp/elosd"}"

export ELOS_SYSLOG_PATH=${ELOS_SYSLOG_PATH:-$SMOKETEST_TMP_DIR/elosd_syslog.socket}
export ELOS_KMSG_FILE=${ELOS_KMSG_FILE:-$SMOKETEST_TMP_DIR/elosd_kmsg.log}
export ELOS_STORAGE_BACKEND_JSON_FILE=${ELOS_STORAGE_BACKEND_JSON_FILE:-$SMOKETEST_TMP_DIR/elosd_storage.json}
export ELOS_SCANNER_PATH=${ELOS_SCANNER_PATH-"$DIST_DIR/usr/local/lib/elos/scanner"}
export ELOS_BACKEND_PATH=${ELOS_BACKEND_PATH-"$DIST_DIR/usr/local/lib/elos/backend"}
export ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH-"$SMOKETEST_DIR/config.json"}
export ELOS_LOG_LEVEL=DEBUG

#config file path for log4c demo
export LOG4C_RCPATH="${BASE_DIR}/src/demo/elos_log4c_demo"

mkdir -p "$SMOKETEST_TMP_DIR"

#change PS1 for the interactive bash and start it
env PS1="[elos-interactive]\$ " /bin/bash --norc -i

#after exiting the bash ...

#kill all instances of elosc
CLIENT_PIDS=$(pgrep elosc | sed 's/^\s\+//' | cut -f1 -d " ")
for pid in $CLIENT_PIDS
do
    kill "$pid"
    echo "killed client with pid $pid"
done

#kill elosd
ELOSD_PID=$(pgrep elosd | sed 's/^\s\+//' | cut -f1 -d " ")
if [ "$ELOSD_PID" != "" ]; then
    kill "$ELOSD_PID"
    echo "killed elosd with pid $ELOSD_PID"
fi
