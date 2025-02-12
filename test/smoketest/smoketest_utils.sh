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

escape_html() {
    sed -e 's/&/\&amp;/g' \
        -e 's/</\&lt;/g' \
        -e 's/>/\&gt;/g' \
        -e 's/"/\&quot;/g' \
        -e "s/'/\&apos;/g"
}

create_junit_report() {
    SOURCE_DIR="${1}"
    REPORT_FILE="${2-${SOURCE_DIR}/junit.xml}"

    truncate -s 0 "${REPORT_FILE}"

    {
        echo '<?xml version="1.0" encoding="UTF-8"?>'
        echo '<testsuites time="0">'
        echo '<testsuite name="smoketests">'
    } >> "${REPORT_FILE}"

    for test in "$SOURCE_DIR"/*/smoketest.result; do

        IFS=' ' read -r TEST_NAME TEST_TIME TEST_STATUS < "${test}"

        printf '<testcase name="%s" time="%s">\n' "${TEST_NAME}" "${TEST_TIME}" >> "${REPORT_FILE}"

        case "${TEST_STATUS}" in
            OK)
                ;;
            SKIPPED)
                echo '<skipped></skipped>' >> "${REPORT_FILE}"
                ;;
            *)
                {
                    echo '<failure>'
                    escape_html < "${SOURCE_DIR}/${TEST_NAME}/smoketest.log"
                    echo '</failure>'
                } >> "${REPORT_FILE}"
                ;;
        esac 

        {
            echo '<system-out>'
            for _LOG_FILE in "${SOURCE_DIR}/${TEST_NAME}/"*.log; do
                echo
                echo "###### ${_LOG_FILE} ######"
                echo
                escape_html < "${_LOG_FILE}"
            done
            echo '</system-out>'
        } >> "${REPORT_FILE}"

        echo '</testcase>' >> "${REPORT_FILE}"
    done

    {
        echo '</testsuite>'
        echo '</testsuites>'
    } >> "${REPORT_FILE}"
}
