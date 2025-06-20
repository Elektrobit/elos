#!/bin/sh

BASE_DIR=$(dirname "$0")/../..
export BUILD_TYPE="${BUILD_TYPE-Debug}"
export BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE/"
export DIST_DIR="${DIST_DIR-"${BUILD_DIR}/dist"}"

# Prepend to library search path if libraries needed by the benchmark test are not
# located in the usually directories.
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-"${DIST_DIR}/usr/local/lib/"}"

# Prepend to PATH if binaries needed by the benchmark test are not located in the
# usually directories.
export PATH="${PATH}:${DIST_DIR}/usr/local/bin/"

# The BENCHMARK_DIR should point to the directory where the benchmark test script and
# its resources are located.
export BENCHMARK_DIR="${BENCHMARK_DIR-"$(dirname "$0")"}"

# The BENCHMARK_RESULT_DIR should pint to a directory where the test result can
# be stored. The default is in the current directory.
export BENCHMARK_RESULT_DIR="${BENCHMARK_RESULT_DIR-"$BUILD_DIR/result/benchmark_results"}"

# The BENCHMARK_TMP_DIR should point to a location where the test can store
# temporary data like files, pipes, sockets. The default is
# /tmp/elos_benchmark_XXXXXX, where XXXXXX is a random number
export BENCHMARK_TMP_DIR="${BENCHMARK_TMP_DIR-"$(mktemp -d /tmp/elos_benchmark_XXXXXX)"}"

# Overwrite the path to the elos runtime directory.
export elos_RunDir="${BENCHMARK_TMP_DIR}/"

# Overwrite the path to the syslog socket to be used for the syslog scanner.
export ELOS_SYSLOG_PATH="${ELOS_SYSLOG_PATH-"/tmp/elosd.syslog.socket"}"

# Overwrite the path to read the kernel log buffer, used by the kmsg scanner.
export ELOS_KMSG_FILE="${ELOS_KMSG_FILE-"/tmp/elosd.kmsg"}"

# Overwrite the client plugin search path to look for client plugins.
export ELOS_CLIENT_PATH="${ELOS_SCANNER_PATH-"$DIST_DIR/usr/local/lib/elos/client"}"

# Overwrite the scanner plugin search path to look for scanner plugins.
export ELOS_SCANNER_PATH="${ELOS_SCANNER_PATH-"$DIST_DIR/usr/local/lib/elos/scanner"}"

# Overwrite the backend plugin search path to look for backend plugins.
export ELOS_BACKEND_PATH="${ELOS_BACKEND_PATH-"$DIST_DIR/usr/local/lib/elos/backend"}"

# Overwrite the log storage path for elos json backend .
export ELOS_STORAGE_BACKEND_JSONBACKEND_FILE="${ELOS_STORAGE_BACKEND_JSONBACKEND_FILE-"/tmp/elosd_event.log"}"

# Overwrite the elos config location for benchmark tests.
export ELOS_CONFIG_PATH="${ELOS_CONFIG_PATH-"$BENCHMARK_DIR/config.json"}"
