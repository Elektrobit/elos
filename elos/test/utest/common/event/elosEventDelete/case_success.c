// SPDX-License-Identifier: MIT
#include "elosEventDelete_utest.h"

int elosTestElosEventDeleteSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeleteSuccess(UNUSED void **state) {
    elosEvent_t *testEvent = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosEventDelete");
    SHOULD("%s", "free an elosEvent_t Object");

    result = elosEventNew(&testEvent);
    assert_int_equal(result, SAFU_RESULT_OK);

    testEvent->payload = strdup("testabc");
    testEvent->hardwareid = strdup("hardwareTest");
    testEvent->source.fileName = strdup("testFileName");
    testEvent->source.appName = strdup("testAppName");

    result = elosEventDelete(testEvent);
    assert_int_equal(result, SAFU_RESULT_OK);
}
