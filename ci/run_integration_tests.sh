#!/bin/bash
###############################################################################
print_info() {
    SCRIPT_NAME="${0##*/}"
    echo "
    Run the integration tests or parts of it. Default is to run all integration
    tests in Debug build.

    Usage: ${SCRIPT_NAME} [BUILD_TYPE] [--test-regex|-R <test name pattern>] [-h|--help]

    BUILD_TYPE		usually Debug or Release but can be any other build type
    -a|--all		run all integration tests (default)
    -m|--module MODULE	Run a test module
    -s|--suite SUITE	Run a test suite
	example: -s \"Tests.Clients.Elosc.Find Event\"
    -c|--case CASE	Run a single test case
    -h|--help:		print this help

    Examples:
    ${0} Release # run all unit test on Release build
    ${0} Release -R elosRpn # run all unit test containing elosRpn in
    the name for the Release build.
    "
}
###############################################################################

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}

TEST_PARAM=("--all")
PARAM=""
while [ $# -gt 0 ]; do
    case ${1} in
	--all|-a)
	    TEST_PARAM=("--all") ;;
	--module|-m)
	    TEST_PARAM=("--module" "${2}")
	    shift
	    ;;
	--suite|-s)
	    TEST_PARAM=("--suite" "${2}")
	    shift
	    ;;
	--case|-c)
	    TEST_PARAM=("--case" "${2}")
	    shift
	    ;;
	--help|-h)
	    print_info
	    exit 0
	    ;;
	-*)
	    echo "error: unknown option: $1"; exit 1 ;;
	*)
	    PARAM="$PARAM ${1}" ;;
    esac
    shift
done
set -- $PARAM
BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"

PROJECT=${BASE_DIR##*/}

# filter the project name to avoid problems when using it with docker containers
PROJECT=$(\
    echo "${PROJECT}" \
    | tr -c '[:alnum:].-' '-' \
    | tr '[:upper:]' '[:lower:]' \
    | sed 's/^[^[:alnum:]]*//;s/[^[:alnum:]]*$//' \
    | sed 's/-\{2,\}/-/g' \
)

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
    ${ELOS_DEPENDENCY_CONFIG:+--build-arg ELOS_DEPENDENCY_CONFIG="${ELOS_DEPENDENCY_CONFIG}"} \
    --tag ${ELOSD_IMAGE_NAME} -f $BASE_DIR/ci/Dockerfile.elosd .

if [ $? -ne 0 ]; then
    echo "Failed to build ${ELOSD_IMAGE_NAME} image"
    exit 1
fi


docker build \
    $BUILD_ARG \
    --build-arg UID=$(id -u) --build-arg GID=$(id -g) \
    --build-arg SOURCES_URI="${SOURCES_URI}" \
    ${ELOS_DEPENDENCY_CONFIG:+--build-arg ELOS_DEPENDENCY_CONFIG="${ELOS_DEPENDENCY_CONFIG}"} \
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
	--env "TARGET_NAME=${ELOSD_DOCKER_NAME}" \
	--env "TEST_OUTPUT=/base/build/${BUILD_TYPE}/result/integration" \
	${TEST_IMAGE_NAME}

if [ $? -ne 0 ]; then
    echo "Failed to run ${TEST_DOCKER_NAME} container"
    tearDown
    exit 1
fi

docker exec ${TEST_DOCKER_NAME} /base/test/integration/scripts/run_integration_tests.sh "${TEST_PARAM[@]}"

ret=$?

tearDown

exit $ret
