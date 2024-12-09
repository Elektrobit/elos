// SPDX-License-Identifier: MIT

#include "elosEventDispatcherStop_utest.h"

int testElosEventDispatcherStopErrParamSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherStopErrParamTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherStopErrParam(UNUSED void **state) {
    elosEventDispatcher_t invalidEventDispatcher = {0};
    safuResultE_t result;

    TEST("elosEventDispatcherStop");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventDispatcherStop(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher");
    result = elosEventDispatcherStop(&invalidEventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
