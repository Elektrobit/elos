#!/bin/sh -e
#
# create and run docker build env
#
CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
. "$BASE_DIR/ci/common_names.sh"
IMAGE_NAME="${PROJECT}"
TARGET_NAME="${PROJECT}-target"

. "$BASE_DIR/ci/dependency_sources.sh"

echo "==> create docker image"
cd $BASE_DIR
docker build \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --build-arg CMAKE_PARAM="${CMAKE_PARAM}" \
    --build-arg SOURCES_URI="${SOURCES_URI}" \
    --tag ${IMAGE_NAME} -f $BASE_DIR/ci/Dockerfile.elosd .

cd $BASE_DIR/ci
echo "==> run $PROJECT container"

if ! [ -e "$BASE_DIR"/ci/sshconfig ]; then
    { echo "Host *"
      echo "  User $(id -u -n)"
    } > "$BASE_DIR"/ci/sshconfig
fi

if [ "$SSH_AUTH_SOCK" ]; then
    SSH_AGENT_SOCK=$(readlink -f $SSH_AUTH_SOCK)
    SSH_AGENT_OPTS="-v $SSH_AGENT_SOCK:/run/ssh-agent -e SSH_AUTH_SOCK=/run/ssh-agent"
fi

docker run --rm -it --cap-add=SYS_ADMIN --security-opt apparmor=unconfined $SSH_AGENT_OPTS \
    --privileged \
    --name ${TARGET_NAME} \
    -w / \
    ${IMAGE_NAME} $@
