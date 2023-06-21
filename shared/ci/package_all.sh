#!/bin/sh
set -x
for proj in cmocka_extensions cmocka_mocks safu samconf elos; do
    cd /base/$proj/
    ci/build.sh --package Release
    ci/package.sh
done
