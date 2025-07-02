#!/bin/sh
###############################################################################
print_info() {
    echo "
    Build and run a docker image providing a ready to use build environment.

    Usage: ${0} [-h|--help] [parameter forwarded to container run]

    -h|--help:        print this help

    Environment variables:

    CI                     - set to true enable CI specific options like supress
                             '-ti' on running the container and sourcing
                             ignored_sources.sh
    GIT_USER_TOKEN         - token to be used to authenticate on http git request
    SOURCES_URI            - overwrite the default source URI
                             https://github.com/Elektrobit/
    ELOS_DEPENDENCY_CONFIG - overwrite dependency config file to use 
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
. "$BASE_DIR/ci/common_names.sh"
IMAGE_NAME="${PROJECT}-build"

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

if [ -n "${SSH_AUTH_SOCK:-}" ]; then
    SSH_AGENT_SOCK=$(readlink -f $SSH_AUTH_SOCK)
    SSH_AGENT_OPTS="-v $SSH_AGENT_SOCK:/run/ssh-agent -e SSH_AUTH_SOCK=/run/ssh-agent"
fi

docker run --rm ${IT} \
    -v $BASE_DIR:/base \
    -w /base \
    ${SSH_AGENT_OPTS:+${SSH_AGENT_OPTS}}  \
    ${GIT_USER_TOKEN:+-e GIT_USER_TOKEN="${GIT_USER_TOKEN}"} \
    ${ELOS_DEPENDENCY_CONFIG:+-e ELOS_DEPENDENCY_CONFIG="${ELOS_DEPENDENCY_CONFIG}"} \
    ${SOURCES_URI:+-e SOURCES_URI="${SOURCES_URI}"} \
    ${UNUSED_SOURCES:+-e UNUSED_SOURCES="${UNUSED_SOURCES}"} \
    ${IGNORE_SOURCES:+-e IGNORE_SOURCES="${IGNORE_SOURCES}"} \
    --privileged \
    --device=/dev/kmsg \
    $IMAGE_NAME $@
