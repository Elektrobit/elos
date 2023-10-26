#!/bin/sh -eu

CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"

echo "remove build directories"
rm -rf \
  "$BASE_DIR/build" \
  "$BASE_DIR/doc/build" \
  "$BASE_DIR/doc/source/generated"
