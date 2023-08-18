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

rm -rf $BUILD_DIR/result/integration
mkdir -p $BUILD_DIR/result/integration

docker build \
    $BUILD_ARG \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --tag ${ELOSD_IMAGE_NAME} -f $BASE_DIR/ci/Dockerfile.elosd .

docker build \
    $BUILD_ARG \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --tag ${TEST_IMAGE_NAME} -f $BASE_DIR/../shared/ci/Dockerfile.robot .


if [ "$SSH_AUTH_SOCK" ]; then
    SSH_AGENT_SOCK=$(readlink -f $SSH_AUTH_SOCK)
    SSH_AGENT_OPTS="-v $SSH_AGENT_SOCK:/run/ssh-agent -e SSH_AUTH_SOCK=/run/ssh-agent"
fi

ELOSD_ID=$(docker run -d -ti --rm \
        --name ${ELOSD_DOCKER_NAME} \
	--cap-add=SYS_ADMIN \
	--security-opt apparmor=unconfined \
	$SSH_AGENT_OPTS \
	--privileged \
	-w / \
	${ELOSD_IMAGE_NAME})

RUNNER_ID=$(docker run -d -ti --rm \
        --name ${TEST_DOCKER_NAME} \
        --link ${ELOSD_DOCKER_NAME} \
	-v $BASE_DIR/..:/base \
	-w /base \
	--env "PROJECT=${PROJECT}" \
	--env "TEST_OUTPUT=/base/elos/build/${BUILD_TYPE}/result/integration" \
	${TEST_IMAGE_NAME})

docker exec ${TEST_DOCKER_NAME} /base/elos/test/integration/scripts/run_integration_tests.sh

ret=$?

docker stop ${ELOSD_ID}
docker stop ${RUNNER_ID}

exit $?
