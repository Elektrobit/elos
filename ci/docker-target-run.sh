#!/bin/sh -e
###############################################################################
print_info() {
    echo "
    Build and run a docker image from the current source and install the
    application.
    It creates a sshconfig and forwards the host ssh-agent into the container.

    Usage: ${0} [-h|--help] [parameter forwarded to container run]

    -h|--help:        print this help

    Environment variables:

    CI                     - set to true enable CI specific options like supress
                             '-ti' on running the container and sourcing
                             ignored_sources.sh
    SOURCES_URI            - overwrite the default source URI
                             https://github.com/Elektrobit/
    CMAKE_PARAM            - provide a list of cmake parmaters to forward to
                             cmake builds in the conatiner
    ELOS_DEPENDENCY_CONFIG - overwrite dependency config file to use 
    ENV_OPTIONS            - define a list of `-e key=value` pairs of environment
                             varialbes to pass into the running container
    SSH_AUTH_SOCK          - if set, the container is startet using the hosts
                             ssh-agent.

    Examples:
    ${0} # build and run container with build environment
    "
}
###############################################################################
set -e -u -o pipefail
CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
RESULT_DIR="${BASE_DIR}/build/Docker/result"
. "$BASE_DIR/ci/common_names.sh"
IMAGE_NAME="${PROJECT}"
TARGET_NAME="${PROJECT}-target"

PARAM=""
while [ $# -gt 0 ]; do
    case ${1} in
        -h|--help)
            print_info
            exit 0 ;;
        *)
            PARAM="$PARAM ${1}" ;;
    esac
    shift
done
set -- $PARAM

IT="-it"
CONTAINER_USE_UID_GID="--build-arg UID=$(id -u) --build-arg GID=$(id -g)"
if [ "${CI:-false}" = true ]; then
    echo "Running in CI"
    . "$BASE_DIR/ci/ignored_sources.sh"
    IT=""
    unset CONTAINER_USE_UID_GID
fi

echo "==> create docker image"
cd $BASE_DIR
docker build \
    ${CONTAINER_USE_UID_GID:+${CONTAINER_USE_UID_GID}} \
    ${CMKAKE_PARAM:+--build-arg CMAKE_PARAM="${CMAKE_PARAM}"} \
    ${SOURCES_URI:+--build-arg SOURCES_URI="${SOURCES_URI}"} \
    ${ELOS_DEPENDENCY_CONFIG:+--build-arg ELOS_DEPENDENCY_CONFIG="${ELOS_DEPENDENCY_CONFIG}"} \
    --tag ${IMAGE_NAME} -f $BASE_DIR/ci/Dockerfile.elosd .

cd $BASE_DIR/ci
echo "==> run $PROJECT container"

if ! [ -e "$BASE_DIR"/ci/sshconfig ]; then
    { echo "Host *"
      echo "  User $(id -u -n)"
    } > "$BASE_DIR"/ci/sshconfig
fi

if [ -n "${SSH_AUTH_SOCK:-}" ]; then
    SSH_AGENT_SOCK=$(readlink -f $SSH_AUTH_SOCK)
    SSH_AGENT_OPTS="-v $SSH_AGENT_SOCK:/run/ssh-agent -e SSH_AUTH_SOCK=/run/ssh-agent"
fi

mkdir -p "${RESULT_DIR}"
chmod o+rw "${RESULT_DIR}"
docker run --rm ${IT} \
    --name ${TARGET_NAME} \
    --privileged \
    --cap-add=SYS_ADMIN \
    --security-opt apparmor=unconfined \
    ${SSH_AGENT_OPTS:+${SSH_AGENT_OPTS}} \
    ${ENV_OPTIONS:+${ENV_OPTIONS}} \
    -v "${RESULT_DIR}:/tmp/results:rw" \
    -w / \
    ${IMAGE_NAME} $@
