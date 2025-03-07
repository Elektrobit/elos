// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosEventDispatcherBufferAdd_utest.h"

int testElosEventDispatcherBufferAddSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherBufferAddSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    test->eventBuffer[0].permitRemoval = true;
    test->eventBuffer[1].permitRemoval = true;
    elosEventDispatcherBufferRemove(&test->eventDispatcher, &test->eventBuffer[0]);
    elosEventDispatcherBufferRemove(&test->eventDispatcher, &test->eventBuffer[1]);
    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherBufferAddSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBuffer_t **eventBuffer;
    safuResultE_t result;
    size_t idx;

    TEST("elosEventDispatcherBufferAdd");
    SHOULD("test behaviour of elosEventDispatcherBufferAdd");

    PARAM("Add first EventBuffer");
    idx = 0;
    result = elosEventDispatcherBufferAdd(&test->eventDispatcher, &test->eventBuffer[0]);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(safuVecElements(&test->eventDispatcher.eventBufferPtrVector), 1);
    eventBuffer = safuVecGetLast(&test->eventDispatcher.eventBufferPtrVector);
    assert_ptr_not_equal(*eventBuffer, NULL);
    assert_int_equal((*eventBuffer)->ringCount, ELOS_EVENTBUFFER_PRIORITY_COUNT);
    assert_int_equal((*eventBuffer)->ring[0].limitEventCount, (idx + 1));
    assert_int_equal((*eventBuffer)->writeTrigger, test->eventDispatcher.worker.trigger);

    PARAM("Add second EventBuffer");
    idx = 1;
    result = elosEventDispatcherBufferAdd(&test->eventDispatcher, &test->eventBuffer[1]);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(safuVecElements(&test->eventDispatcher.eventBufferPtrVector), 2);
    eventBuffer = safuVecGetLast(&test->eventDispatcher.eventBufferPtrVector);
    assert_ptr_not_equal(*eventBuffer, NULL);
    assert_int_equal((*eventBuffer)->ringCount, ELOS_EVENTBUFFER_PRIORITY_COUNT);
    assert_int_equal((*eventBuffer)->ring[0].limitEventCount, (idx + 1));
    assert_int_equal((*eventBuffer)->writeTrigger, test->eventDispatcher.worker.trigger);
}
