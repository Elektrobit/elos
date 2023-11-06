#!/bin/sh -e
#
# create and run docker build env
#
CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
. "$BASE_DIR/ci/common_names.sh"
IMAGE_NAME="${PROJECT}-build"

IT="-it"
if [ "${CI}" = true ]; then
    echo "Running in CI"
    . "$BASE_DIR/ci/ignored_sources.sh"
    IT=""
fi

if [ -f "${HOME}/.ssh/asmcov_token" ]; then
    echo "Loading asmcov token from ${HOME}/.ssh/asmcov_token."
    . "${HOME}/.ssh/asmcov_token"
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

docker run --rm ${IT} $SSH_AGENT_OPTS \
    --net elos-net \
    -v $BASE_DIR:/base \
    -w /base \
    ${GIT_USER_TOKEN:+-e GIT_USER_TOKEN="${GIT_USER_TOKEN}"} \
    ${ELOS_DEPENDENCY_CONFIG:+-e ELOS_DEPENDENCY_CONFIG="${ELOS_DEPENDENCY_CONFIG}"} \
    ${SOURCES_URI:+-e SOURCES_URI="${SOURCES_URI}"} \
    ${UNUSED_SOURCES:+-e UNUSED_SOURCES="${UNUSED_SOURCES}"} \
    ${IGNORE_SOURCES:+-e IGNORE_SOURCES="${IGNORE_SOURCES}"} \
    --privileged \
    --device=/dev/kmsg \
    $IMAGE_NAME $@
