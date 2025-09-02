#!/bin/bash 
CMDPATH=$(realpath "$(dirname "$0")")

. "${CMDPATH}/smoketest_env.sh"

mkdir -p "$SMOKETEST_TMP_DIR"

#initialize the dlt shared memory ring buffer
mng_dlt_buffer -c 10 \
    -f "${elos_Scanner_Plugins_DltHv_Config_DeviceFile}" \
    -o "${elos_Scanner_Plugins_DltHv_Config_OffsetAddress}" \
    -s "${elos_Scanner_Plugins_DltHv_Config_BufferSize}"

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
#unlink the dlt shared memory ring buffer again
mng_dlt_buffer -u \
    -f "${elos_Scanner_Plugins_DltHv_Config_DeviceFile}" \
 
