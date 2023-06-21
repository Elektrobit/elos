#!/bin/sh -x

TESTS=$(find /usr/local/lib/test/elos -type f -executable)

#skip failing test case
#FAILTESTPATH="
# /opt/emlix/lib/test/elos/client_manager/test_clientManager_elosClientManagerStart_utest
#"
#
#for i in $FAILTESTPATH; do
#    echo "skip $i"
#done

FAIL=0
for test in $TESTS; do
    CMOCKA_MESSAGE_OUTPUT="TAP" $test
    if [ $? -ne 0 ]; then
        echo "Test failed: $test"
        let FAIL++
    fi
done

exit $FAIL
