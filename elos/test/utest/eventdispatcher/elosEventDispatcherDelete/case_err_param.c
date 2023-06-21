// SPDX-License-Identifier: MIT

#include "elosEventDispatcherDelete_utest.h"

int testElosEventDispatcherDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherDeleteErrParam(UNUSED void **state) {
    elosEventDispatcher_t invalidEventDispatcher = {0};
    elosEventDispatcher_t *invalidEventDispatcherPtr = &invalidEventDispatcher;
    safuResultE_t result;

    TEST("elosEventDispatcherDelete");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventDispatcherDelete(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher");
    result = elosEventDispatcherDelete(&invalidEventDispatcherPtr);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
