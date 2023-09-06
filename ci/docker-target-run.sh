#!/bin/sh -e
#
# create and run docker build env
#
CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}
PROJECT=${BASE_DIR##*/}
IMAGE_NAME="${PROJECT}"
TARGET_NAME="${PROJECT}-target"

CMOCKA_EXTENSIONS_REPO_NAME=${CMOCKA_EXTENSIONS_REPO_NAME:-cmocka-extensions.git}
CMOCKA_EXTENSIONS_REPO_PATH=${CMOCKA_EXTENSIONS_REPO_PATH:-${SOURCES_URI}/${CMOCKA_EXTENSIONS_REPO_NAME}}
CMOCKA_EXTENSIONS_REPO_REF=${CMOCKA_EXTENSIONS_REPO_REF:-integration}

CMOCKA_MOCKS_REPO_NAME=${CMOCKA_MOCKS_REPO_NAME:-cmocka-mocks.git}
CMOCKA_MOCKS_REPO_PATH=${CMOCKA_MOCKS_REPO_PATH:-${SOURCES_URI}/${CMOCKA_MOCKS_REPO_NAME}}
CMOCKA_MOCKS_REPO_REF=${CMOCKA_MOCKS_REPO_REF:-integration}

SAFU_REPO_NAME=${SAFU_REPO_NAME:-safu.git}
SAFU_REPO_PATH=${SAFU_REPO_PATH:-${SOURCES_URI}/${SAFU_REPO_NAME}}
SAFU_REPO_REF=${SAFU_REPO_REF:-integration}

SAMCONF_REPO_NAME=${SAMCONF_REPO_NAME:-samconf.git}
SAMCONF_REPO_PATH=${SAMCONF_REPO_PATH:-${SOURCES_URI}/${SAMCONF_REPO_NAME}}
SAMCONF_REPO_REF=${SAMCONF_REPO_REF:-integration}

CMAKE_PARAM=""
CMAKE_PARAM="${CMAKE_PARAM} -D CMOCKA_EXTENSIONS_URI=${CMOCKA_EXTENSIONS_REPO_PATH} \
                            -D CMOCKA_EXTENSIONS_REF=${CMOCKA_EXTENSIONS_REPO_REF}"
CMAKE_PARAM="${CMAKE_PARAM} -D CMOCKA_MOCKS_URI=${CMOCKA_MOCKS_REPO_PATH} \
                            -D CMOCKA_MOCKS_REF=${CMOCKA_MOCKS_REPO_REF}"
CMAKE_PARAM="${CMAKE_PARAM} -D SAFU_URI=${SAFU_REPO_PATH} \
                            -D SAFU_REF=${SAFU_REPO_REF}"
CMAKE_PARAM="${CMAKE_PARAM} -D SAMCONF_URI=${SAMCONF_REPO_PATH} \
                            -D SAMCONF_REF=${SAMCONF_REPO_REF}"

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
