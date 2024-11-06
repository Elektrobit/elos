# SPDX-License-Identifier: MIT

is_listening_on() {
    PID="${1}"
    PORT_AS_HEX="$(printf '%X' "${2}")"
    if [ ! -d "/proc/${PID}/fd" ] || [ ! -e "/proc/${PID}/net/tcp" ]; then
        echo "/proc/${PID} missing entries"
        return 0;
    fi

    INODES="$(grep ":${PORT_AS_HEX} " "/proc/${PID}/net/tcp" | tr -s ' ' | cut -d ' ' -f 11 | tr '\n' ' ')"
    for INODE in $INODES; do
        if [ "${INODE}" -ne 0 ]; then
            if find  "/proc/${PID}/fd/" -exec readlink {} \; | grep "socket:\[${INODE}\]" ; then
                return 1;
            fi 
        fi
    done

    return 0;
}

get_parent_pid() {
    if [ $# -eq 0 ]; then
        PID="$$"
    else
        PID="${1}"
    fi
    cut -f 4 -d ' ' "/proc/${PID}/stat"
}
