# Point to a directory where to expect the typical bin, lib and include
# directories
export PREFIX_PATH="${PREFIX_PATH-"/usr"}"

# Prepend to PATH if binaries needed by the smoke test are not located in the
# usually directories.
export PATH="${PATH}:${PREFIX_PATH}/bin"

# Prendnd to library search path if libraries needed by the smoke test are not
# located in the usually directories.
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-""}:${PREFIX_PATH}/lib"

# Prepend to pkg-config search path if pkg-config files are needed by the
# smoke test are not located in the usually directories.
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH-""}:${PREFIX_PATH}/lib/pkgconfig"

# The SMOKETEST_DIR should point to the directory where the smoke test script and
# its resources are located. The default assumes the directory where
# smoke test.sh is located (sets the CMDPATH).
export SMOKETEST_DIR=${SMOKETEST_DIR-${CMDPATH}}

# The SMOKETEST_RESULT_DIR should pint to a directory where the test result can
# be stored. The default is in the current directory.
export SMOKETEST_RESULT_DIR=${SMOKETEST_RESULT_DIR-"./results/smoketest"}

# The SMOKETEST_TMP_DIR should point to a location where the test can store
# temporary data like files, pipes, sockets. The default is
# /tmp/elos_smoketest_XXXXXX, where XXXXXX is a random number
export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR-"$(mktemp -d /tmp/elos_smoketest_XXXXXX)"}"

# Overwrite the path to the elos runtime directory.
export elos_RunDir="${SMOKETEST_TMP_DIR}/"

# Overwrite the elosd log level, be verbose for the smoketest.
export elos_LogLevel=DEBUG

# Overwrite the coredump config file path.
export ELOS_COREDUMP_CONFIG_FILE=${ELOS_COREDUMP_CONFIG_FILE:-"${SMOKETEST_DIR}/coredump.json"}


## ======= ClientInput Configurations =======

# Overwrite the client plugin search path to look for client plugins.
export elos_ClientInputs_PluginSearchPath="${PREFIX_PATH}/lib/elos/client"

# Overwrite the path to the elos unix socket to be used for the unix plugin.
export elos_ClientInputs_Plugins_unixClient_Config_path="${SMOKETEST_TMP_DIR}/elosd.socket"

# Overwrite the TCP port to be used for the smoketest
#export ELOSD_PORT=${ELOSD_PORT-54323}
export elos_ClientInputs_Plugins_PublicTcpClient_Config_Port=${ELOSD_PORT-54323}
export elos_ClientInputs_Plugins_LocalTcp_Config_Port=${ELOSD_PORT-54322}


## ======= Scanner Configurations =======

# Overwrite the path to the syslog socket to be used for the syslog scanner.
export elos_Scanner_Plugins_SyslogScanner_Config_SyslogPath="${SMOKETEST_TMP_DIR}/elosd.syslog.socket"

# Overwrite the path to read the kernel log buffer, used by the kmsg scanner.
export elos_Scanner_Plugins_ScannerKmsg_Config_KmsgFile="${SMOKETEST_TMP_DIR}/elosd.kmsg"

# Overwrite the scanner plugin search path to look for scanner plugins.
export elos_Scanner_PluginSearchPath="${PREFIX_PATH}/lib/elos/scanner"


## ======= Backend Configurations =======

# Overwrite the backend plugin search path to look for backend plugins.
export elos_EventLogging_PluginSearchPath="${PREFIX_PATH}/lib/elos/backend"

# Overwrite the file-path used for the DLT fifo for the DLT backend.
export elos_EventLogging_Plugins_DLT_Config_Connection="${SMOKETEST_TMP_DIR}/dlt"


# Take care of special sysconfdir cases
if [ "${PREFIX_PATH}" = "/usr" ] || [ "${PREFIX_PATH}" = "/" ]; then
    export SYSCONF_DIR="/etc"
else
    export SYSCONF_DIR="${PREFIX_PATH}/etc"
fi

# Overwrite the elosd config file path.
export ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH-"$SMOKETEST_DIR"}

# Overwrite the log4crc file path for the log4c demo.
export LOG4C_RCPATH="${SYSCONF_DIR}/elos/elos_log4c_demo"

# Use SMOKETEST_ENABLE_COMPILE_TESTS to enable complie test. This
# tests if all necessary header and library files are packet for
# installation. Set SMOKETEST_ENABLE_COMPILE_TESTS to any value
# other then empty to activate the tests.
export SMOKETEST_ENABLE_COMPILE_TESTS="${SMOKETEST_ENABLE_COMPILE_TESTS-""}"
