#!/bin/bash
set -eou pipefail

BASE_DIR=$(realpath $(dirname $0)/../../)

BUILD_TYPE="${2:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"

DIST_DIR=$(realpath "$BUILD_DIR/dist/")

export LD_LIBRARY_PATH="$DIST_DIR/usr/local/lib"
export PATH=${PATH}:"$BUILD_DIR/cmake/src/demo:$DIST_DIR/usr/local/bin:$BASE_DIR/src"
export SMOKETEST_DIR=${SMOKETEST_DIR-"$(dirname $0)"}
export SMOKETEST_RESULT_DIR=${SMOKETEST_RESULT_DIR-"$BUILD_DIR/result/smoketest_results"}
export PRIVATE_KEY="$SMOKETEST_DIR/samconf.pem"
export PUBLIC_KEY="$SMOKETEST_DIR/samconf.pub"
export SAMCONF_SIGNATURE_KEY="$PUBLIC_KEY"

function smoketest_simple_config {
    set -x
    RESULT_DIR="$SMOKETEST_RESULT_DIR/simple_config"
    rm -rvf $RESULT_DIR
    mkdir -p $RESULT_DIR

    echo "Starting smoketest simple config ..."
    samprobe "$SMOKETEST_DIR"/config.json &> $RESULT_DIR/simple_config_output.txt

    if [ $? -ne 0 ]; then
        error_exit "samprobe failed"
    fi

    echo -ne "Signature file ${SMOKETEST_DIR}/config.json.sig does not exist.\n\n" \
        > ${RESULT_DIR}/simple_config_nosign_output.txt
    cat ${SMOKETEST_DIR}/simple_config_output.txt \
        >> ${RESULT_DIR}/simple_config_nosign_output.txt
    
    echo "Smoketest comparing output ${RESULT_DIR}/simple_config_output.txt with ${RESULT_DIR}/simple_config_nosign_output.txt"
    output_diff=$(diff -w ${RESULT_DIR}/simple_config_output.txt ${RESULT_DIR}/simple_config_nosign_output.txt || echo "diff returned: $?")
    if [ -n "$output_diff" ]; then
        echo "Problems occured while comparing the client output:"
        echo -e "$output_diff"
        error_exit "Test failed"
    fi

    echo "Finished smoketest."
    exit 0
}

function smoketest_signed_config {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/signed_config"
    rm -rvf $RESULT_DIR
    mkdir -p $RESULT_DIR
    cp -a "$SMOKETEST_DIR/config.json" "$RESULT_DIR/config.json"
    local CONFIG_FILE="$RESULT_DIR/config.json"

    echo "Starting smoketest signed config ..."
    signature.sh $CONFIG_FILE "$PRIVATE_KEY"
    samprobe $CONFIG_FILE &> $RESULT_DIR/signed_config_output.txt
    re=$?
    if [ $re -ne 0 ]; then
        error_exit "samprobe with signature failed"
    fi

    echo "Finished smoketest."
    exit 0
}

function smoketest_error_signed_config {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/error_signed_config"
    rm -rvf $RESULT_DIR
    mkdir -p $RESULT_DIR
    cp -a "$SMOKETEST_DIR/config.json" "$RESULT_DIR/config.json"
    local CONFIG_FILE="$RESULT_DIR/config.json"

    echo "Starting smoketest error signed config ..."
    echo -n "RSA-SHA2-256:///1234567890123456789012345678901234567890123456789012345678901234" > $CONFIG_FILE.sig
    set +e
    samprobe $CONFIG_FILE &> $RESULT_DIR/error_signed_config_output.txt
    re=$?
    set -e
    if [ $re -eq 0 ]; then
        error_exit "samprobe with wrong signature succeeded"
    fi

    echo "Smoketest check output for expected error"
    grep -q "ERR: Signature was invalid"  $RESULT_DIR/error_signed_config_output.txt
    if [ $? -ne 0 ]; then
        echo "samprobe failed with wrong error:"
        cat $RESULT_DIR/error_signed_config_output.txt
        error_exit "Test failed"
    fi

    echo "Finished smoketest."
    exit 0
}

function smoketest_sign_config {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/sign_config"
    rm -rvf $RESULT_DIR
    mkdir -p $RESULT_DIR
    cp -a "$SMOKETEST_DIR/config.json" "$RESULT_DIR/config.json"

    echo "Starting smoketest signing config ..."


    signature.sh "$RESULT_DIR/config.json" "$PRIVATE_KEY"
    if [ $? -ne 0 ]; then
        error_exit "signing failed: executable returned error code on exit"
    fi

    if [ ! -f "$RESULT_DIR/config.json.sig" ]; then
        error_exit "signing failed: signature file doesnt exist"
    fi

    OUTPUT=$(cat $RESULT_DIR/config.json.sig | tr -d "\n")
    EXPECTED_OUTPUT="RSA-SHA2-256:///vc7o5/wo8qPZ9aJiSYuJK/s7Ba4kZQA1luddm+YLozpuD1AT2YtkiasFKITbI3xJnWmeUFfv9k1mgJ0MEXNK/IF8T89lMSxk7eXze4zR32FwjKtjCrs7n4EU27+427QNE6C1tqmmxpwS07PUBA3ngRCa4n1LhrTciZWOEUMLTqAYsE72zd7yV4z0AMR5b1OhUuiKMpw2TLBypMRQZ9bQ63tXG4tBHpdCwq7c4C2OQLbbKB0OyrANg4MCSRYcghnx3Zks1I2NQCGoaK9ir8oYiHHM81UBREvbrT+1ghiu+Xf4O8NNfegSRM79CRsEyf0p23Qvil7+5RrxuspDBzvgnQ=="
    if [ "$OUTPUT" != "$EXPECTED_OUTPUT" ]; then
        echo "signing failed:"
        echo "$OUTPUT(generated) != $EXPECTED_OUTPUT(expected)"
        error_exit "Test failed"
    fi

    echo "Finished smoketest."
    exit 0
}

function smoketest_genkeys {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/genkeys"
    rm -rvf $RESULT_DIR
    mkdir -p $RESULT_DIR

    echo "Starting smoketest signing config ..."
    signature.sh create_keys "$RESULT_DIR" "samconf"
    if [ $? -ne 0 ]; then
        error_exit "creating a key pair failed"
    fi

    if [ ! -f $RESULT_DIR/samconf.pem ] || [ ! -s $RESULT_DIR/samconf.pem ]; then
        error_exit "expected private key file does not exist or is empty : $RESULT_DIR/samconf.pem"
    fi

    if [ ! -f $RESULT_DIR/samconf.pub ] || [ ! -s $RESULT_DIR/samconf.pub ]; then
        error_exit "expected public key file does not exist or is empty : $RESULT_DIR/samconf.pub"
    fi

    echo "Finished smoketest."
    exit 0
}

function print_help {
    echo
    echo "Usage: $0 <simple_config|sign_config|help> <Debug|Release>"
    echo
}

function error_exit {
    echo "Error : $1, terminating smoketest ..."
    exit 1
}

if [ $# -lt 1 ]; then
    print_help
    exit 1
fi

mkdir -p $SMOKETEST_RESULT_DIR

case $1 in
    simple_config)
        smoketest_simple_config
        ;;
    signed_config)
        smoketest_signed_config
        ;;
    error_signed_config)
        smoketest_error_signed_config
        ;;
    sign_config)
        smoketest_sign_config
        ;;
    genkeys)
        smoketest_genkeys
        ;;
    help)
        print_help
        ;;
    *)
        print_help
        exit 1
        ;;
esac
