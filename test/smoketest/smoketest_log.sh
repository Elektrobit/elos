# SPDX-License-Identifier: MIT

setup_log() {
    LOG_DIR="${1}"
	if [ ! -d "${LOG_DIR}" ]; then
		echo "No Result dir available! Exitting"
		exit 2
	fi
    FILE="${LOG_DIR}/smoketest.log"
	echo "" > "$FILE"
}

log() {
	printf '%s\n' "$1" >> "$FILE"
}

log_err() {
	printf '[ERROR] %s\n' "$1" >> "$FILE"
}
