// SPDX-License-Identifier: MIT

#include "elosEventBufferInitialize_utest.h"

#define _TEST_EVENTLIMITCOUNT 8

int elosTestElosEventBufferInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventBufferInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventBufferDeleteMembers(&test->eventBuffer);

    return 0;
}

void elosTestElosEventBufferInitializeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventBufferParam_t const param = {
        .limitEventCount = _TEST_EVENTLIMITCOUNT,
    };

    TEST("elosEventBufferInitialize");
    SHOULD("%s", "test correct behaviour of elosEventBufferInitialize");

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->eventBuffer.flags), true);
    assert_int_equal(test->eventBuffer.ringCount, ELOS_EVENTBUFFER_PRIORITY_COUNT);

    for (int i = 0; i < ELOS_EVENTBUFFER_PRIORITY_COUNT; i += 1) {
        elosEventBufferRing_t *ring = &test->eventBuffer.ring[i];

        // The RingBuffer has its own unit tests, so we test only one of the parameters here
        assert_int_equal(ring->ringBuffer.memorySize, _TEST_EVENTLIMITCOUNT * sizeof(elosEvent_t *));
        assert_int_equal(ring->limitEventCount, _TEST_EVENTLIMITCOUNT);
    }
}
