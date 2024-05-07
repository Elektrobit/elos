#!/bin/bash -e

urlencode() {
  local string="${1}"
  string="${string// /%20}"   # Encode spaces as %20
  string="${string//\//%2F}"  # Encode slashes as %2F
  string="${string//+/%2B}"   # Encode plus signs as %2B
  string="${string//&/%26}"   # Encode ampersands as %26
  string="${string//\?/%3F}"  # Encode question marks as %3F
  string="${string//=/%3D}"   # Encode equal signs as %3D
  string="${string//=/%3D}"   # Encode equal signs as %3D
  string="${string//#/%23}"   # Encode pound signs as %23
  echo "${string}"
}

JOB_NAME="eb-baseos-elos-baseos-lab"
PARAMETERS="ELOS_DEV_BRANCH=$(urlencode "${BRANCH_NAME}")"

triggerBuild() {
    local QUEUE_URL
    QUEUE_URL=$(curl -X POST -u "${JENKINS_JOB_USER}:${JENKINS_JOB_TOKEN}" "${JENKINS_URI}/job/${JOB_NAME}/buildWithParameters?${PARAMETERS}" -s -D - -o /dev/null | grep -i 'Location' | awk '{print $2}' | tr -d '\r')
    QUEUE_ID=$(basename "${QUEUE_URL}")
    if [ -n "$QUEUE_ID" ]; then
        echo "Triggered job ${JOB_NAME} with '${PARAMETERS}'. Queue ID: ${QUEUE_ID}"
    else
        echo "Failed to trigger job ${JOB_NAME}"
        exit 1
    fi
}

waitForJobToStart() {
    echo "Waiting for the job to start building..."
    while true; do
        BUILD_URL=$(curl -s -u "${JENKINS_JOB_USER}:${JENKINS_JOB_TOKEN}" "${JENKINS_URI}/queue/item/${QUEUE_ID}/api/json" | jq -r '.executable.url')
        if [ "${BUILD_URL}" != "null" ]; then
            break
        fi
        sleep 10
    done
    local BUILD_NUMBER
    BUILD_NUMBER=$(basename "${BUILD_URL}")
    if [ -n "${BUILD_NUMBER}" ]; then
        echo "Job ${JOB_NAME} started building. Build number: ${BUILD_NUMBER}"
    else
        echo "Failed to retrieve build number for job ${JOB_NAME}"
        exit 1
    fi
}

waitForJobToFinish() {
    local BUILD_STATUS
    echo "Build run as ${BUILD_URL}"
    echo "Waiting for the job to finish..."
    while true; do
        BUILD_STATUS=$(curl -s -u "${JENKINS_JOB_USER}:${JENKINS_JOB_TOKEN}" "${BUILD_URL}/api/json" | jq -r '.result')
        if [ "${BUILD_STATUS}" != "null" ]; then
            break
        fi
        sleep 10
    done
    echo "Pipeline job ${JOB_NAME} finished with status: ${BUILD_STATUS}"
    if [ "${BUILD_STATUS}" == "SUCCESS" ]; then
        return 0
    else
        return 1
    fi
}

printHelp() {
    echo "Usage: ${0}"
    echo ""
    echo "Required environment variables: "
    echo ""
    echo "JENKINS_URI: base URI to acces the jenikins instance (i.e \`https://jenkins.local\`)"
    echo "JENKINS_JOB_USER: the username to use for authorization"
    echo "JENKINS_JOB_TOKEN: the access token to use for authorization"
    echo "BRANCH_NAME: the name of the elos branch that shall be build as plain text NOT escaped or urlencoded"
}

if [ "${#}" == "1" ] && [ "${1}" == "-h" ] || [ "${1}" == "--help" ]; then
  printHelp
  exit 0
fi

if [ -z "${JENKINS_URI}" ] \
    || [ -z "${JENKINS_JOB_USER}" ] \
    || [ -z "${JENKINS_JOB_TOKEN}" ] \
    || [ -z "${BRANCH_NAME}" ]; then
  printHelp
  exit 1
fi

triggerBuild
waitForJobToStart
waitForJobToFinish
