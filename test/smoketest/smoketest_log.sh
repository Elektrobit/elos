# SPDX-License-Identifier: MIT

FILE="${SMOKETEST_RESULT_DIR}/smoketest.log"

setup_log() {
	if [ "${SMOKETEST_RESULT_DIR}" = "" ]; then
		echo "No Result dir available! Exitting" >&3
		exit 2
	fi
	echo "" > "$FILE"
}

log() {
	printf '%s\n' "$1" >> "$FILE"
}

log_err() {
	printf '[ERROR] %s\n' "$1" >> "$FILE"
}
