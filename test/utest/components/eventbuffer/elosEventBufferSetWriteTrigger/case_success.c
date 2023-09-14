// SPDX-License-Identifier: MIT

#include "elosEventBufferSetWriteTrigger_utest.h"

#define _TEST_EVENTLIMITCOUNT    8
#define _TEST_WRITETRIGGER_VALUE 42

int elosTestElosEventBufferSetWriteTriggerSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENTLIMITCOUNT};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferSetWriteTriggerSuccessTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosEventBufferDeleteMembers(&test->eventBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

void elosTestElosEventBufferSetWriteTriggerSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    int writeTrigger = -1;

    TEST("elosEventBufferSetWriteTrigger");
    SHOULD("%s", "test correct behaviour of elosEventBufferSetWriteTrigger");

    PARAM("%s", "test normal function operation");
    result = elosEventBufferSetWriteTrigger(&test->eventBuffer, _TEST_WRITETRIGGER_VALUE);
    assert_int_equal(result, SAFU_RESULT_OK);
    writeTrigger = atomic_load(&test->eventBuffer.writeTrigger);
    assert_int_equal(writeTrigger, _TEST_WRITETRIGGER_VALUE);
}
