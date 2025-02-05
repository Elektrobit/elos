// SPDX-License-Identifier: MIT

#include "elosEventDispatcherBufferRemove_utest.h"

int testElosEventDispatcherBufferRemoveErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherBufferRemoveErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherBufferRemoveErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcher_t invalidEventDispatcher = {0};
    elosEventBuffer_t unknownEventBuffer = {0};
    safuResultE_t result;

    test->eventBuffer[0].permitRemoval = true;

    TEST("elosEventDispatcherBufferRemove");
    SHOULD("%s", "test behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventDispatcherBufferRemove(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, eventBuffer");
    result = elosEventDispatcherBufferRemove(NULL, &test->eventBuffer[0]);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher, NULL");
    result = elosEventDispatcherBufferRemove(&invalidEventDispatcher, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventDispatcher, eventBuffer");
    result = elosEventDispatcherBufferRemove(&invalidEventDispatcher, &test->eventBuffer[0]);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventDispatcher, NULL");
    result = elosEventDispatcherBufferRemove(&test->eventDispatcher, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "eventDispatcher, unknownEventBuffer");
    result = elosEventDispatcherBufferRemove(&test->eventDispatcher, &unknownEventBuffer);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
