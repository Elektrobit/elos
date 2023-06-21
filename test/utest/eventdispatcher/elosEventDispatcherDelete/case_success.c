// SPDX-License-Identifier: MIT

#include "elosEventDispatcherDelete_utest.h"

int testElosEventDispatcherDeleteSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherNew(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherDeleteSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventDispatcherDelete");
    SHOULD("%s", "test correct behaviour of elosEventDispatcherDelete");

    PARAM("%s", "test normal function operation");
    result = elosEventDispatcherDelete(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(test->eventDispatcher, NULL);
}
