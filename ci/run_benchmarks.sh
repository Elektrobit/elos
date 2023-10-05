#!/bin/bash

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
BUILD_TYPE="${1:-Debug}"
. "$BASE_DIR/ci/common_names.sh"

BENCHMARK_RESULT_DIR=${BENCHMARK_RESULT_DIR:-"$RESULT_DIR/benchmark_results"}

export BUILD_TYPE
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
