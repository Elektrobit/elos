#!/bin/sh -x

TESTS=$(find /usr/local/lib/test/samconf -type f -executable)

FAIL=0
for test in $TESTS; do
    CMOCKA_MESSAGE_OUTPUT="TAP" $test
    if [ $? -ne 0 ]; then
        echo "Test failed: $test"
        let FAIL++
    fi
done

exit $FAIL
