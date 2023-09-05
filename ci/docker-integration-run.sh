#!/bin/sh -e
#
# create and run docker build env
#
CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}
PROJECT=${BASE_DIR##*/}
IMAGE_NAME="${PROJECT}-robot"
TARGET_NAME="${PROJECT}-target"

echo "==> create docker image"
cd $BASE_DIR/ci
docker build \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --tag ${IMAGE_NAME} -f Dockerfile.robot .

echo "==> run $PROJECT robot container"

if ! [ -e "$BASE_DIR"/ci/sshconfig ]; then
    { echo "Host *"
      echo "  User $(id -u -n)"
    } > "$BASE_DIR"/ci/sshconfig
fi

if [ "$SSH_AUTH_SOCK" ]; then
    SSH_AGENT_SOCK=$(readlink -f $SSH_AUTH_SOCK)
    SSH_AGENT_OPTS="-v $SSH_AGENT_SOCK:/run/ssh-agent -e SSH_AUTH_SOCK=/run/ssh-agent"
fi

docker run --rm -it $SSH_AGENT_OPTS \
    --name host \
    --link $TARGET_NAME \
    -v $BASE_DIR/test/integration:/${PROJECT}/test/integration \
    -w /${PROJECT}/test/integration \
    $IMAGE_NAME $@
