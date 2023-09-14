// SPDX-License-Identifier: MIT
#include "elosEventDispatcherDispatch_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(testElosEventDispatcherDispatchErrFunc),
        TEST_CASE(testElosEventDispatcherDispatchErrMutex),
        TEST_CASE(testElosEventDispatcherDispatchErrParam),
        TEST_CASE(testElosEventDispatcherDispatchSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        elosEventBufferParam_t const param = {0};
        result = elosEventBufferInitialize(&test->eventBuffer[i], &param);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    test->event[_EVENT_IDX_NORMAL].severity = ELOS_SEVERITY_INFO;
    test->event[_EVENT_IDX_NORMAL].payload = _EVENT_PAYLOAD_NORMAL;
    test->event[_EVENT_IDX_HIGH].severity = ELOS_SEVERITY_FATAL;
    test->event[_EVENT_IDX_HIGH].payload = _EVENT_PAYLOAD_HIGH;

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        elosEventBufferDeleteMembers(&test->eventBuffer[i]);
    }

    free(*state);
    *state = NULL;

    return 0;
}

void elosTestEventBufferReset(elosUnitTestState_t *test) {
    safuResultE_t result;

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        elosEventBufferDeleteMembers(&test->eventBuffer[i]);
    }

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        elosEventBufferParam_t const param = {0};
        result = elosEventBufferInitialize(&test->eventBuffer[i], &param);
        assert_int_equal(result, SAFU_RESULT_OK);
    }
}

safuResultE_t elosMockEventProcessorPublish(UNUSED elosEventProcessor_t *eventProcessor, elosEvent_t const *event) {
    safuResultE_t result;

    MOCK_FUNC_DATA_NEW(data, elosMockData_t, elosEventProcessorPublish);
    result = data->result;

    if (result == SAFU_RESULT_OK) {
        if (event->severity == ELOS_SEVERITY_FATAL) {
            data->eventCount.high += 1;
            assert_string_equal(event->payload, _EVENT_PAYLOAD_HIGH);
        } else if (event->severity == ELOS_SEVERITY_INFO) {
            data->eventCount.normal += 1;
            assert_string_equal(event->payload, _EVENT_PAYLOAD_NORMAL);
        }

        printf("Published Event {.severity:%u, .payload:'%s'}\n", event->severity, event->payload);
    }

    return result;
}
