// SPDX-License-Identifier: MIT
#include "elosJsonBackendFindEvents_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosJsonBackendFindEventsUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosJsonBackendFindEventsSuccess),
    };
    return RUN_TEST_SUITE(tests, elosJsonBackendFindEventsUtest);
}

void elosJsonBackendFindEventsUtestCreateEvent(void **state) {
    elosUteststateT_t *testState = *state;

    elosEventSource_t testEventSource = {
        .appName = "testApp",
        .fileName = "testFile",
        .pid = 123,
    };

    const elosEvent_t testEvent = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = testEventSource,
        .severity = 0,
        .hardwareid = "localhost",
        .classification = 42,
        .messageCode = 32,
        .payload = "payload",
    };

    safuResultE_t retval = elosEventSerialize(&testState->eventJsonString, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
    testState->eventStrLen = strlen(testState->eventJsonString);
}

void elosJsonBackendFindEventsUtestCreateFilter(void **state) {
    elosUteststateT_t *testState = *state;
    int errVal = 0;

    const char *filterRules[NUM_FILTER] = {
        ".event.source.appName 'testApp' STRCMP",
        ".event.source.pid 123 EQ",
        ".event.messageCode 0 EQ",
    };

    testState->filterRules = safuAllocMem(NULL, sizeof(filterRules));
    assert_non_null(testState->filterRules);
    memcpy(testState->filterRules, filterRules, sizeof(filterRules));

    elosEventFilter_t *filter = safuAllocMem(NULL, NUM_FILTER * sizeof(elosEventFilter_t));
    assert_non_null(filter);

    elosEventFilterParam_t param = {0};

    for (int idx = 0; idx < NUM_FILTER; idx++) {
        param.filterString = filterRules[idx];
        assert_ptr_not_equal(param.filterString, NULL);

        errVal = elosEventFilterCreate(&filter[idx], &param);
        assert_int_not_equal(errVal, RPNFILTER_RESULT_ERROR);
    }

    testState->filter = filter;
}

static int elosJsonBackendFindEventsUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosJsonBackendFindEventsUtestTeardown(UNUSED void **state) {
    return 0;
}
