// SPDX-License-Identifier: MIT

#include <pthread.h>

#include "elosEventBufferWrite_utest.h"

#define _TEST_ELEMENTS          (2 * ELOS_EVENTBUFFER_PRIORITY_COUNT)
#define _TEST_WORKERS           (4)
#define _TEST_WORKER_ITERATIONS (_TEST_ELEMENTS * 100)
#define _TEST_TOTAL_ITERATIONS  (_TEST_WORKERS * _TEST_WORKER_ITERATIONS)

void *_workerThread(void *state) {
    elosUnitTestState_t *test = (elosUnitTestState_t *)state;

    for (size_t i = 0; i < _TEST_WORKER_ITERATIONS; i += 1) {
        size_t const priority = (i % ELOS_EVENTBUFFER_PRIORITY_COUNT);
        safuResultE_t result;
        elosEvent_t event = {
            .messageCode = i,
            .severity = ELOS_SEVERITY_INFO,
            .payload = "workerTestPayload",
        };

        if (priority == ELOS_EVENTBUFFER_PRIORITY_HIGH) {
            event.severity = ELOS_SEVERITY_FATAL;
        }

        result = elosEventBufferWrite(&test->eventBuffer, &event);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    return NULL;
}

int elosTestElosEventBufferWriteSuccessThreadedSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_ELEMENTS};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferWriteSuccessThreadedTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventBufferDeleteMembers(&test->eventBuffer);

    return 0;
}

void elosTestElosEventBufferWriteSuccessThreaded(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    pthread_t worker[_TEST_WORKERS] = {0};

    TEST("safuRingBufferWriteThreaded");
    SHOULD("%s", "test correct behaviour of multi-threaded safuRingBufferWrite");

    for (size_t i = 0; i < _TEST_WORKERS; i += 1) {
        int retVal;

        retVal = pthread_create(&worker[i], NULL, _workerThread, *state);
        assert_int_equal(retVal, 0);
    }

    for (size_t i = 0; i < _TEST_WORKERS; i += 1) {
        int retVal;

        retVal = pthread_join(worker[i], NULL);
        assert_int_equal(retVal, 0);
    }

    elosEventBufferRing_t *ring;
    size_t const elementsWritten = (_TEST_TOTAL_ITERATIONS / ELOS_EVENTBUFFER_PRIORITY_COUNT);
    size_t const finalNextIdx = (elementsWritten % _TEST_ELEMENTS);

    ring = &test->eventBuffer.ring[ELOS_EVENTBUFFER_PRIORITY_NORMAL];
    assert_int_equal(ring->ringBuffer.elementsWritten, elementsWritten);
    assert_int_equal(ring->ringBuffer.nextIdx, finalNextIdx);

    ring = &test->eventBuffer.ring[ELOS_EVENTBUFFER_PRIORITY_HIGH];
    assert_int_equal(ring->ringBuffer.elementsWritten, elementsWritten);
    assert_int_equal(ring->ringBuffer.nextIdx, finalNextIdx);
}
