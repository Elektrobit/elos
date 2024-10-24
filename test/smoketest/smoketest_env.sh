export PREFIX_PATH="${PREFIX_PATH-"/usr"}"
export PATH="${PATH}:${PREFIX_PATH}/bin"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-""}:${PREFIX_PATH}/lib"
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH-""}:${PREFIX_PATH}/lib/pkgconfig"

export SMOKETEST_DIR=${SMOKETEST_DIR-${CMDPATH}}
export SMOKETEST_RESULT_DIR=${SMOKETEST_RESULT_DIR-"./results/smoketest"}
export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR-"$(mktemp -d /tmp/elos_smoketest_XXXXXX)"}"

export ELOS_SYSLOG_PATH=${ELOS_SYSLOG_PATH-"${SMOKETEST_TMP_DIR}/elosd.syslog.socket"}
export ELOS_KMSG_FILE=${ELOS_KMSG_FILE-"${SMOKETEST_TMP_DIR}/elosd.kmsg"}
export ELOS_DLT_PIPE_PATH=${ELOS_DLT_PIPE_PATH-"${SMOKETEST_TMP_DIR}/dlt"}
export ELOS_CLIENT_PATH=${ELOS_CLIENT_PATH-"${PREFIX_PATH}/lib/elos/client"}
export ELOS_SCANNER_PATH=${ELOS_SCANNER_PATH-"${PREFIX_PATH}/lib/elos/scanner"}
export ELOS_BACKEND_PATH=${ELOS_BACKEND_PATH-"${PREFIX_PATH}/lib/elos/backend"}
export ELOS_COREDUMP_CONFIG_FILE=${ELOS_COREDUMP_CONFIG_FILE:-"$PREFIX_PATH/etc/elos/coredump.json"}
export ELOS_LOG_LEVEL=DEBUG

export ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH-"$SMOKETEST_DIR/config.json"}

# take care of special sysconfdir cases
if [ "${PREFIX_PATH}" = "/usr" ] || [ "${PREFIX_PATH}" = "/" ]; then
    export LOG4C_RCPATH="/etc/elos/elos_log4c_demo"
else
    export LOG4C_RCPATH="${PREFIX_PATH}/etc/elos/elos_log4c_demo"
fi
