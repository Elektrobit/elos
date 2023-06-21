#!/bin/bash

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

export BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
BENCHMARK_RESULT_DIR=${BENCHMARK_RESULT_DIR:-"$BUILD_DIR/result/benchmark_results"}

export BENCHMARK_RESULT_DIR
export BUILD_DIR

$BASE_DIR/test/benchmark/fullstack_1-1.sh
$BASE_DIR/test/benchmark/fullstack_1-n.sh

FAIL=0
for CSV_FILE in $(find ${BENCHMARK_RESULT_DIR} -name "*.csv" -type f); do
    LINE_COUNT=$(cat $CSV_FILE | wc -l)
    if [ ${LINE_COUNT} -lt 2 ]; then
        echo "No results for ${CSV_FILE}"
        let FAIL++;
    else
        echo "Generate plot for '$CSV_FILE' .."
        gnuplot -e report_file=\'$CSV_FILE\' $CMD_PATH/plot_csv_report.p
    fi
done

exit ${FAIL}
