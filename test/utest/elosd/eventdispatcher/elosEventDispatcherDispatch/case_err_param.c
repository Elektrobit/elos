// SPDX-License-Identifier: MIT

#include "elosEventDispatcherDispatch_utest.h"

int testElosEventDispatcherDispatchErrParamSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherDispatchErrParamTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherDispatchErrParam(UNUSED void **state) {
    elosEventDispatcher_t invalidEventDispatcher = {0};
    safuResultE_t result;

    TEST("elosEventDispatcherDispatch");
    SHOULD("%s", "test behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventDispatcherDispatch(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher, NULL");
    result = elosEventDispatcherDispatch(&invalidEventDispatcher);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
