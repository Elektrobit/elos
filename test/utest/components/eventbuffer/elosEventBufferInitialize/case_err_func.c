// SPDX-License-Identifier: MIT

#include "elosEventBufferInitialize_utest.h"
#include "safu/mock_ringbuffer.h"

#define _TEST_EVENTLIMITCOUNT 8

int elosTestElosEventBufferInitializeErrFuncSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventBufferInitializeErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferDeleteMembers(&test->eventBuffer);
    return 0;
}

void elosTestElosEventBufferInitializeErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventBufferParam_t const param = {
        .limitEventCount = _TEST_EVENTLIMITCOUNT,
    };

    TEST("elosEventBufferInitialize");
    SHOULD("%s", "test correct behaviour with failed function calls");

    PARAM("%s", "safuRingBufferWrite fails");
    MOCK_FUNC_ALWAYS(safuRingBufferInitialize);
    expect_not_value(__wrap_safuRingBufferInitialize, ringBuffer, NULL);
    expect_not_value(__wrap_safuRingBufferInitialize, param, NULL);
    will_return(__wrap_safuRingBufferInitialize, SAFU_RESULT_FAILED);

    PARAM("%s", "safuRingBufferInitialize fails");
    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    MOCK_FUNC_NEVER(safuRingBufferInitialize);
}
