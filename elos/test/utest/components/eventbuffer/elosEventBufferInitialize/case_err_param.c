// SPDX-License-Identifier: MIT

#include "elosEventBufferInitialize_utest.h"

#define _TEST_EVENTLIMITCOUNT 8

int elosTestElosEventBufferInitializeErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventBufferParam_t const param = {
        .limitEventCount = _TEST_EVENTLIMITCOUNT,
    };

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferInitializeErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferDeleteMembers(&test->eventBuffer);
    return 0;
}

void elosTestElosEventBufferInitializeErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBuffer_t eventBuffer = {0};
    safuResultE_t result;
    elosEventBufferParam_t const param = {
        .limitEventCount = _TEST_EVENTLIMITCOUNT,
    };

    TEST("elosEventBufferInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventBufferInitialize(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, param");
    result = elosEventBufferInitialize(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventFilterManager, NULL");
    result = elosEventBufferInitialize(&eventBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "initializedEventBuffer, NULL");
    result = elosEventBufferInitialize(&test->eventBuffer, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
