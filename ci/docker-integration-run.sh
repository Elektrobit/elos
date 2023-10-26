#!/bin/sh -e
#
# create and run docker build env
#
CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
. "$BASE_DIR/ci/common_names.sh"
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
    -v $BASE_DIR:/base \
    -w /base \
    $IMAGE_NAME $@
