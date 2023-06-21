// SPDX-License-Identifier: MIT

#include "elosEventBufferDelete_utest.h"

#define _TEST_ID              42
#define _TEST_EVENTLIMITCOUNT 8

int elosTestElosEventBufferDeleteSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENTLIMITCOUNT};
    safuResultE_t result;

    result = elosEventBufferNew(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventBufferDeleteSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventBufferDelete");
    SHOULD("%s", "test correct behaviour of elosEventBufferDelete");

    PARAM("%s", "test normal function operation");
    result = elosEventBufferDelete(&test->eventBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(test->eventBuffer, NULL);
}
