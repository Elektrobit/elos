#!/bin/sh
# SPDX-License-Identifier: MIT
###############################################################################
print_info() {
    SCRIPT_NAME="${0##*/}"
    echo "
Removes all but the last N elosd json log files

Usage: ${SCRIPT_NAME} [OPTIONS] [STORAGE_PATH]

Options:
 -k, --keep <NUMBER>  keep NUMBER log files and delete the rest (default 10)
 -H, --host <HOST>    only delete logs with that specified host id
 -d, --dryrun         don't delete anything just show what would be kept
 -h, --help           print this help

Examples:
 ${SCRIPT_NAME}      # delete all files matching the pattern \"/tmp/elosd_*_*_*.log\"
 ${SCRIPT_NAME} -k 1 # only keep the currently active log file
"
}
###############################################################################
set -eu

STORAGE_PATH="/tmp/elosd_%host%_%date%_%count%.log"
HOST="*"
KEEP="10"
DRYRUN=0

while [ $# -gt 0 ]; do
	case ${1} in
		--keep | -k)   KEEP="${2}"; shift ;;
		--host | -H)   HOST="${2}"; shift ;;
		--dryrun | -d) DRYRUN=1 ;;
		--help | -h)
			print_info
			exit 0
			;;
		-*)
			echo "error: unknown option: ${1}"
			print_info
			exit 1
			;;
		*) STORAGE_PATH="${1}" ;;
	esac
	shift
done

STORAGE_PATH="$(echo "${STORAGE_PATH}" | sed -e "s/%host%/${HOST}/g" -e 's/%date%/*/g' -e 's/%count%/*/g')"

if [ "${DRYRUN}" -ne 0 ]; then
	echo "# Keeping:"
	ls -1t ${STORAGE_PATH} | head -n 10
fi

KEEP=$(($KEEP + 1))
if [ "${DRYRUN}" -eq 0 ]; then
	for log in $(ls -1t ${STORAGE_PATH} | tail -n "+${KEEP}"); do
		rm "${log}"
	done
fi
