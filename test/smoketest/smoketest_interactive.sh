CMDPATH=$(realpath "$(dirname "$0")")
BASE_DIR=$(realpath "${CMDPATH}/../..")
BUILD_TYPE="${1:-Debug}"

. ${BASE_DIR}/test/smoketest/smoketest_env.sh

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
