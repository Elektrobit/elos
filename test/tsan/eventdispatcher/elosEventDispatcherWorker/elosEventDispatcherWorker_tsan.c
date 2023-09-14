// SPDX-License-Identifier: MIT

#include "elosEventDispatcherWorker_tsan.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(testElosEventDispatcherWorkerSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        elosEventBufferParam_t const param = {.limitEventCount = _TEST_TOTAL_ITERATIONS};
        result = elosEventBufferInitialize(&test->eventBuffer[i], &param);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    size_t high = 0;
    size_t normal = 0;

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        normal += test->eventBuffer[i].ring[0].ringBuffer.elementsWritten;
        high += test->eventBuffer[i].ring[1].ringBuffer.elementsWritten;

        elosEventBufferDeleteMembers(&test->eventBuffer[i]);
    }

    free(*state);
    *state = NULL;

    return 0;
}

elosEventProcessorPublishBandaid_t elosEventProcessorPublishBandaid = {
    .result = SAFU_RESULT_OK,
    .eventCount = {.high = 0, .normal = 0, .health = 0},
};

safuResultE_t elosEventProcessorPublish(UNUSED elosEventProcessor_t *eventProcessor, elosEvent_t const *event) {
    safuResultE_t result = elosEventProcessorPublishBandaid.result;

    if (result == SAFU_RESULT_OK) {
        if (event->classification & ELOS_CLASSIFICATION_ELOS) {
            atomic_fetch_add(&elosEventProcessorPublishBandaid.eventCount.health, 1);
        } else if (event->severity == ELOS_SEVERITY_FATAL) {
            atomic_fetch_add(&elosEventProcessorPublishBandaid.eventCount.high, 1);
        } else if (event->severity == ELOS_SEVERITY_INFO) {
            atomic_fetch_add(&elosEventProcessorPublishBandaid.eventCount.normal, 1);
        }
    }

    return result;
}
