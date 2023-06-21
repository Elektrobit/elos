#!/bin/bash
# SPDX-License-Identifier: MIT

# Create an signature file for a given file path argument
set -e

help () {
    echo "signature.sh – manage samconf signatures"
    echo
    echo "sign a given file with private key:"
    echo "  $>signature.sh <valid_path> <private key path>"
    echo
    echo "create a key pair to use with samconf:"
    echo "  $>signature.sh create_keys <output directory> <keyname>"
}

signfile () {
    if [ $# -ne 2 ];then
        help
        exit 1
    fi

    local FILE=$1
    local PRIVATE_KEY=$2

    if test -f "${FILE}"; then
        echo -n "RSA-SHA2-256:///$(openssl dgst -sha256 -sign $PRIVATE_KEY -binary $FILE | openssl enc -A -base64)" > ${FILE}.sig
    else
        help
        echo ""
        echo "Error: Invalid Filepath:" $FILE
        exit 1
    fi
}

create_keys () {
    if [ $# -ne 2 ];then
        help
        exit 1
    fi

    local OUTPUT_DIR=$1
    local KEY_NAME=$2

    openssl genpkey -algorithm RSA -out "$OUTPUT_DIR/$KEY_NAME.pem"
    openssl rsa -in "$OUTPUT_DIR/$KEY_NAME.pem" -pubout -out "$OUTPUT_DIR/$KEY_NAME.pub"
}

case "${1-xxx}" in
    -h | --help)
        help
        ;;
    create_keys)
        shift
        create_keys $@
        ;;
    xxx)
        help
        exit 1
        ;;
    *)
        signfile $@
        ;;
esac
