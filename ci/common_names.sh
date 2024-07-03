
## to include in any script
# CMD_PATH="$(realpath "$(dirname "$0")")"
# BASE_DIR="$(realpath "$CMD_PATH/..")" # replace .. by the path to project root
# . "$BASE_DIR/ci/common_names.sh"

PROJECT="${PROJECT:-${BASE_DIR##*/}}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
BUILD_DIR="${BUILD_DIR:-${BASE_DIR}/build/${BUILD_TYPE}}"
CMAKE_BUILD_DIR="${CMAKE_BUILD_DIR:-${BUILD_DIR}/cmake}"
DIST_DIR="${DIST_DIR:-"${BUILD_DIR}/dist"}"
RESULT_DIR="${RESULT_DIR:-${BUILD_DIR}/result}"

