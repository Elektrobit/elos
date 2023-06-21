#!/bin/bash

##############################################################################################################
# This script will replace mocking expressions like:
# MOCK_FUNC_VAR_NEW(elosEventVectorNew);
# safuResultE_t MOCK_FUNC_WRAP(elosEventVectorNew)(elosEventVector_t **eventVector, uint32_t elements) {
# 
# With the new pattern:
# MOCK_FUNC_BODY(elosEventVectorNew, safuResultE_t, elosEventVector_t **eventVector, uint32_t elements) {
#
# For some reasons this script must called twice to do that ... ?
##############################################################################################################

USAGE(){
    echo "The first parameter of this script must be a file:" >&2
	echo "$0 <filename>" >&2
    exit 1
}

#Main
if [ $# -ne 1 ]; then
        USAGE
else
	number_of_lines=$(wc -l $1 | cut -d " " -f1)
	i=1	
	while [ $i -lt $number_of_lines ]; do
        line=$(sed "${i}q;d" "$1")

        #delete MOCK_FUNC_VAR_NEW line from file
        if [ $(echo "$line" | grep -c "MOCK_FUNC_VAR_NEW") -gt 0 ]; then
            sed -i "${i}d" $1
        fi

        #replace MOCK_FUNC_WRAP line with MOCK_FUNC_BODY line
        if [ $(echo "$line" | grep -c "MOCK_FUNC_WRAP") -gt 0 ]; then
            return_type="$(echo "${line%%MOCK_FUNC_WRAP*}")"
            sed -i "${i}s/.*MOCK_FUNC_WRAP/MOCK_FUNC_BODY/g" $1
            sed -i "${i}s/)(/, $return_type, /g" $1
            sed -i "${i}s/\,\ )/)/g" $1
        fi
        i=$(( $i + 1 ))
    done
    clang-format -i $1
	exit 0
fi
