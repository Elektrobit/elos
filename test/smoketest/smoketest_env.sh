export BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
export DIST_DIR="${BUILD_DIR}/dist"
export PREFIX_PATH="${DIST_DIR}/usr/local"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-""}:${PREFIX_PATH}/lib:${BASE_DIR}/build/deps/lib"
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH-""}:${PREFIX_PATH}/lib/pkgconfig:${BASE_DIR}/build/deps/lib/pkgconfig"
export PATH="${PATH}:${PREFIX_PATH}/bin"

export TEST_SOURCE_DIR="$BASE_DIR/test/smoketest"
export SMOKETEST_DIR=${SMOKETEST_DIR-$TEST_SOURCE_DIR}
export SMOKETEST_RESULT_DIR=${SMOKETEST_RESULT_DIR-"$BUILD_DIR/result/smoketest_results"}
export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR-"/tmp/elosd"}"

export ELOS_SYSLOG_PATH=${ELOS_SYSLOG_PATH-"${SMOKETEST_TMP_DIR}/elosd.syslog.socket"}
export ELOS_KMSG_FILE=${ELOS_KMSG_FILE-"${SMOKETEST_TMP_DIR}/elosd.kmsg"}
export ELOS_CLIENT_PATH=${ELOS_CLIENT_PATH-"${PREFIX_PATH}/lib/elos/client"}
export ELOS_SCANNER_PATH=${ELOS_SCANNER_PATH-"${PREFIX_PATH}/lib/elos/scanner"}
export ELOS_BACKEND_PATH=${ELOS_BACKEND_PATH-"${PREFIX_PATH}/lib/elos/backend"}
export ELOS_COREDUMP_CONFIG_FILE=${ELOS_COREDUMP_CONFIG_FILE:-"$PREFIX_PATH/etc/elos/coredump.json"}
export ELOS_LOG_LEVEL=DEBUG

export ELOS_CONFIG_PATH=${ELOS_CONFIG_PATH-"$SMOKETEST_DIR/config.json"}
