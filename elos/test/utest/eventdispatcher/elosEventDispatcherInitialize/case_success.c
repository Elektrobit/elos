// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosEventDispatcherInitialize_utest.h"

int testElosEventDispatcherInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherInitializeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuVec_t *vector;
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {
        .eventProcessor = &test->eventProcessor,
    };

    TEST("elosEventDispatcherInitialize");
    SHOULD("%s", "test correct behaviour of elosEventDispatcherInitialize");

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->eventDispatcher.flags), true);
    vector = &test->eventDispatcher.eventBufferPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(void *) * ELOS_EVENTDISPATCHER_DEFAULT_VECTOR_SIZE);
}
