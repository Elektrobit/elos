// SPDX-License-Identifier: MIT

#include "elosEventBufferWrite_utest.h"

#define _TEST_EVENT_LIMIT 8

int elosTestElosEventBufferWriteErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENT_LIMIT};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferWriteErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventBufferDeleteMembers(&test->eventBuffer);

    return 0;
}

void elosTestElosEventBufferWriteErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBuffer_t invalidEventBuffer = {0};
    safuResultE_t result;

    TEST("elosEventBufferWrite");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventBufferWrite(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventBuffer, NULL");
    result = elosEventBufferWrite(&invalidEventBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "EventBuffer, NULL");
    result = elosEventBufferWrite(&test->eventBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
