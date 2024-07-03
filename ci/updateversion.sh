#!/bin/sh -e
#
# Update all version numbers to given version
#

CMD_PATH=$(cd "$(dirname "$0")" && pwd)
BASE_DIR=${CMD_PATH%/*}
PROJECT_NAME=$(basename $BASE_DIR)

INPUTFILE_CMAKE="$BASE_DIR/cmake/project.cmake"

if [ $# -ne 1 ];
then
	echo "Usage: $0 <version>"
	exit 1
fi
VERSION=$1

# update version in cmake project file
sed -i "s,^set(ELOS_VERSION .*\$,set(ELOS_VERSION ${VERSION})," "${INPUTFILE_CMAKE}"

git add -p

git commit -m "elos: Update to ${VERSION}" 
echo "New version:"
git show
