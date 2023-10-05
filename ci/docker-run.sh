#!/bin/sh -e
#
# create and run docker build env
#
CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
. "$BASE_DIR/ci/common_names.sh"
IMAGE_NAME="${PROJECT}-build"

if [ -f "${HOME}/.ssh/asmcov_token" ]; then
    echo "Loading asmcov token from ${HOME}/.ssh/asmcov_token."
    source "${HOME}/.ssh/asmcov_token"
fi

echo "==> create docker image"
cd $BASE_DIR/ci
DOCKER_BUILDKIT=0 \
docker build \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --build-arg ASMCOV_URI="${ASMCOV_URI-not available}" \
    --tag ${IMAGE_NAME} -f Dockerfile .

echo "==> run $PROJECT build container"

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
    -v $BASE_DIR:/base/elos \
    -w /base/elos \
    -e GIT_USER_TOKEN="${GIT_USER_TOKEN}" \
    --privileged \
    --device=/dev/kmsg \
    $IMAGE_NAME $@
