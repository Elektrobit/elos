#!/bin/sh -eu

CMD_PATH=$(cd $(dirname $0) && pwd)
BASE_DIR=${CMD_PATH%/*}
REPO_ROOT_DIR="$BASE_DIR/.."
PROJECT_NAME=$(basename $BASE_DIR)

REPOS="safu samconf elos cmocka_mocks cmocka_extensions cmocka_examples"
for repo in $REPOS; do
  echo "remove build directories for $repo:"
  rm -rf \
    "$REPO_ROOT_DIR/$repo/build" \
    "$REPO_ROOT_DIR/$repo/doc/build" \
    "$REPO_ROOT_DIR/$repo/doc/source/generated"
done
