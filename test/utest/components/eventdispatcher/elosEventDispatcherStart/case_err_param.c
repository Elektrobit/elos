// SPDX-License-Identifier: MIT

#include "elosEventDispatcherStart_utest.h"

int testElosEventDispatcherStartErrParamSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherStartErrParamTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherStartErrParam(UNUSED void **state) {
    elosEventDispatcher_t invalidEventDispatcher = {0};
    safuResultE_t result;

    TEST("elosEventDispatcherStart");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventDispatcherStart(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher");
    result = elosEventDispatcherStart(&invalidEventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
