// SPDX-License-Identifier: MIT

#include "elosEventDispatcherNew_utest.h"

int testElosEventDispatcherNewSuccessSetup(UNUSED void **state) {
    return 0;
}

int testElosEventDispatcherNewSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDelete(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherNewSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuVec_t *vector;
    safuResultE_t result;
    elosEventDispatcherParam_t const param = {
        .eventProcessor = &test->eventProcessor,
    };

    TEST("elosEventDispatcherNew");
    SHOULD("%s", "test correct behaviour of elosEventDispatcherNew");

    result = elosEventDispatcherNew(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->eventDispatcher->flags), true);
    vector = &test->eventDispatcher->eventBufferPtrVector;
    assert_int_equal(vector->elementCount, 0);
    assert_int_equal(vector->memorySize, sizeof(void *) * ELOS_EVENTDISPATCHER_DEFAULT_VECTOR_SIZE);
}
