#!/bin/bash

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"

PROJECT=${BASE_DIR##*/}
ELOSD_IMAGE_NAME="${PROJECT:-elos}"
ELOSD_DOCKER_NAME="${PROJECT}-target"
TEST_IMAGE_NAME="${PROJECT}-robot"
TEST_DOCKER_NAME="${PROJECT}-runner"
BUILD_ARG=${BUILD_ARG:-}

if [ -n "${CI}" ]; then
    BUILD_ID="${BUILD_ID:-'none'}"
    GIT_COMMIT="${GIT_COMMIT:-'none'}"
    ELOSD_IMAGE_NAME="${ELOSD_IMAGE_NAME}-${BUILD_ID}-${GIT_COMMIT}"
    ELOSD_DOCKER_NAME="${ELOSD_DOCKER_NAME}-${BUILD_ID}-${GIT_COMMIT}"
    TEST_IMAGE_NAME="${TEST_IMAGE_NAME}-${BUILD_ID}-${GIT_COMMIT}"
    TEST_DOCKER_NAME="${TEST_DOCKER_NAME}-${BUILD_ID}-${GIT_COMMIT}"
fi

function tearDown {
    echo "TearDown: cleanup "
    docker stop "${ELOSD_DOCKER_NAME}" "${TEST_DOCKER_NAME}"
}

rm -rf $BUILD_DIR/result/integration
mkdir -p $BUILD_DIR/result/integration

docker build \
    $BUILD_ARG \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --build-arg SOURCES_URI="${SOURCES_URI}" \
    --tag ${ELOSD_IMAGE_NAME} -f $BASE_DIR/ci/Dockerfile.elosd .

if [ $? -ne 0 ]; then
    echo "Failed to build ${ELOSD_IMAGE_NAME} image"
    exit 1
fi


docker build \
    $BUILD_ARG \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --build-arg SOURCES_URI="${SOURCES_URI}" \
    --tag ${TEST_IMAGE_NAME} -f $BASE_DIR/ci/Dockerfile.robot .

if [ $? -ne 0 ]; then
    echo "Failed to build ${TEST_IMAGE_NAME} image"
    exit 1
fi

if [ "$SSH_AUTH_SOCK" ]; then
    SSH_AGENT_SOCK=$(readlink -f $SSH_AUTH_SOCK)
    SSH_AGENT_OPTS="-v $SSH_AGENT_SOCK:/run/ssh-agent -e SSH_AUTH_SOCK=/run/ssh-agent"
fi

trap tearDown SIGTERM SIGABRT SIGQUIT SIGINT

docker run -d -ti --rm \
        --name ${ELOSD_DOCKER_NAME} \
	--cap-add=SYS_ADMIN \
	--security-opt apparmor=unconfined \
	$SSH_AGENT_OPTS \
	--privileged \
	-w / \
	${ELOSD_IMAGE_NAME}

if [ $? -ne 0 ]; then
    echo "Failed to run ${ELOSD_DOCKER_NAME} container"
    exit 1
fi

docker run -d -ti --rm \
        --name ${TEST_DOCKER_NAME} \
        --link ${ELOSD_DOCKER_NAME} \
	-v $BASE_DIR:/base \
	-w /base \
	--env "PROJECT=${PROJECT}" \
	--env "TEST_OUTPUT=/base/build/${BUILD_TYPE}/result/integration" \
	${TEST_IMAGE_NAME}

if [ $? -ne 0 ]; then
    echo "Failed to run ${TEST_DOCKER_NAME} container"
    tearDown
    exit 1
fi

docker exec ${TEST_DOCKER_NAME} /base/test/integration/scripts/run_integration_tests.sh

ret=$?

tearDown

exit $ret
