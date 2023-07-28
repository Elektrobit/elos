// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosEventBufferWrite_utest.h"
#include "safu/mock_ringbuffer.h"

#define _TEST_EVENT_LIMIT        8
#define _TEST_EVENT_PAYLOAD      "eventPriorityHigh"
#define _TEST_WRITETRIGGER_VALUE 42

int elosTestElosEventBufferWriteErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENT_LIMIT};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferWriteErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventBufferDeleteMembers(&test->eventBuffer);

    return 0;
}

void elosTestElosEventBufferWriteErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEvent_t event = {.payload = _TEST_EVENT_PAYLOAD, .severity = ELOS_SEVERITY_INFO};
    safuResultE_t result;

    TEST("elosEventBufferWrite");
    SHOULD("%s", "test correct behaviour with failed function calls");

    PARAM("%s", "safuRingBufferWrite fails");
    MOCK_FUNC_ALWAYS(safuRingBufferWrite);
    expect_not_value(__wrap_safuRingBufferWrite, ringBuffer, NULL);
    expect_not_value(__wrap_safuRingBufferWrite, element, NULL);
    will_return(__wrap_safuRingBufferWrite, SAFU_RESULT_FAILED);

    result = elosEventBufferWrite(&test->eventBuffer, &event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    MOCK_FUNC_NEVER(safuRingBufferWrite);

    PARAM("%s", "eventfd_write fails");

    result = elosEventBufferSetWriteTrigger(&test->eventBuffer, _TEST_WRITETRIGGER_VALUE);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(eventfd_write, 0);
    expect_value(__wrap_eventfd_write, __fd, _TEST_WRITETRIGGER_VALUE);
    expect_value(__wrap_eventfd_write, __value, 1);
    will_return(__wrap_eventfd_write, -1);

    result = elosEventBufferWrite(&test->eventBuffer, &event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    MOCK_FUNC_NEVER(safuRingBufferWrite);
}
