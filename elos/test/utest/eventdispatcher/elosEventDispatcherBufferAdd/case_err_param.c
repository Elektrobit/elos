// SPDX-License-Identifier: MIT

#include "elosEventDispatcherBufferAdd_utest.h"

int testElosEventDispatcherBufferAddErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherBufferAddErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherBufferAddErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcher_t invalidEventDispatcher = {0};
    safuResultE_t result;

    TEST("elosEventDispatcherBufferAdd");
    SHOULD("%s", "test behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventDispatcherBufferAdd(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, eventBuffer");
    result = elosEventDispatcherBufferAdd(NULL, &test->eventBuffer[0]);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher, NULL");
    result = elosEventDispatcherBufferAdd(&invalidEventDispatcher, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher, eventBuffer");
    result = elosEventDispatcherBufferAdd(&invalidEventDispatcher, &test->eventBuffer[0]);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventDispatcher, NULL");
    result = elosEventDispatcherBufferAdd(&test->eventDispatcher, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
