// SPDX-License-Identifier: MIT

#include <sched.h>

#include "elosEventDispatcherWorker_tsan.h"

static elosEvent_t const elosEventTable[_EVENT_ENTRIES] = {
    [_EVENT_IDX_NORMAL] = {.severity = ELOS_SEVERITY_INFO, .payload = _EVENT_PAYLOAD_NORMAL},
    [_EVENT_IDX_HIGH] = {.severity = ELOS_SEVERITY_FATAL, .payload = _EVENT_PAYLOAD_HIGH},
};

static atomic_bool elosWorkerRun = false;

void *_workerThread(void *state) {
    elosUnitTestWorker_t *worker = (elosUnitTestWorker_t *)state;
    elosEvent_t const *event = &elosEventTable[_EVENT_IDX_NORMAL];

    if (worker->priority == ELOS_EVENTBUFFER_PRIORITY_HIGH) {
        event = &elosEventTable[_EVENT_IDX_HIGH];
    }

    while (atomic_load(&elosWorkerRun) == false) {
        sched_yield();
    }

    for (size_t i = 0; i < _TEST_WORKER_ITERATIONS; i += 1) {
        safuResultE_t result;

        result = elosEventBufferWrite(worker->eventBuffer, event);
        assert_int_equal(result, SAFU_RESULT_OK);

        sched_yield();
    }

    return NULL;
}

int testElosEventDispatcherWorkerSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {
        .eventProcessor = &test->eventProcessor,
    };
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        result = elosEventDispatcherBufferAdd(&test->eventDispatcher, &test->eventBuffer[i]);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    return 0;
}

int testElosEventDispatcherWorkerSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherWorkerSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    size_t eventCount = _TEST_TOTAL_ITERATIONS / ELOS_EVENTBUFFER_PRIORITY_COUNT;
    safuResultE_t result;

    TEST("elosEventDispatcherWorker");
    SHOULD("%s", "test correct behaviour of multi-threaded EventDispatcher");

    elosEventProcessorPublishBandaid.result = SAFU_RESULT_OK;
    atomic_store(&elosEventProcessorPublishBandaid.eventCount.high, 0);
    atomic_store(&elosEventProcessorPublishBandaid.eventCount.normal, 0);
    atomic_store(&elosEventProcessorPublishBandaid.eventCount.health, 0);

    result = elosEventDispatcherStart(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);

    for (size_t i = 0; i < _TEST_WORKERS; i += 1) {
        elosUnitTestWorker_t *worker = &test->worker[i];
        size_t const idx = (i % _EVENTBUFFER_ENTRIES);
        int retVal;

        worker->eventBuffer = &test->eventBuffer[idx];
        worker->priority = (i % ELOS_EVENTBUFFER_PRIORITY_COUNT);

        retVal = pthread_create(&worker->thread, NULL, _workerThread, worker);
        assert_int_equal(retVal, 0);
    }

    atomic_store(&elosWorkerRun, true);

    for (size_t i = 0; i < _TEST_WORKERS; i += 1) {
        elosUnitTestWorker_t *worker = &test->worker[i];
        int retVal;

        retVal = pthread_join(worker->thread, NULL);
        assert_int_equal(retVal, 0);
    }

    result = elosEventDispatcherStop(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(atomic_load(&elosEventProcessorPublishBandaid.eventCount.normal), eventCount);
    assert_int_equal(atomic_load(&elosEventProcessorPublishBandaid.eventCount.high), eventCount);
    assert_in_range(atomic_load(&elosEventProcessorPublishBandaid.eventCount.health), 1, 256);
}
