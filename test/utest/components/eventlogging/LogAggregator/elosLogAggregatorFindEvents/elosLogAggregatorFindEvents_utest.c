// SPDX-License-Identifier: MIT
#include "elosLogAggregatorFindEvents_utest.h"

#include "elos/eventfilter/eventfilter.h"
#include "elos/eventlogging/StorageBackend.h"
#include "elos/plugincontrol/plugincontrol.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLogAggregatorFindEventsUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLogAggregatorFindEventsErrParamLogAggregatorNull),
        TEST_CASE(elosTestElosLogAggregatorFindEventsErrParamRuleNull),
        TEST_CASE(elosTestElosLogAggregatorFindEventsErrParamEventsNull),
        TEST_CASE(elosTestElosLogAggregatorFindEventsExterrEventFilterCreate),
        TEST_CASE(elosTestElosLogAggregatorFindEventsExterrMutex),
    };
    return RUN_TEST_SUITE(tests, elosLogAggregatorFindEventsUtest);
}

void elosLogAggregatorFindEventsUtestCreateEvent(void **state) {
    elosUteststateT_t *testState = *state;
    elosEvent_t const testEvent = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {.appName = "testApp", .fileName = "testFile", .pid = 123},
        .severity = 0,
        .hardwareid = "localhost",
        .classification = 42,
        .messageCode = 32,
        .payload = "payload",
    };

    safuResultE_t retval = elosEventSerialize(&testState->eventJsonString, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_OK);
}

void elosLogAggregatorFindEventsUtestFreeEvent(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->eventJsonString);
}

void elosLogAggregatorFindEventsUtestCreateFilterRules(void **state) {
    elosUteststateT_t *testState = *state;

    const char *filterRules[NUM_FILTER] = {
        ".event.source.appName 'testApp' STRCMP",
        ".event.source.pid 123 EQ",
        ".event.messageCode 0 EQ",
    };

    testState->filterRules = safuAllocMem(NULL, sizeof(filterRules));
    assert_non_null(testState->filterRules);
    memcpy(testState->filterRules, filterRules, sizeof(filterRules));
}

void elosLogAggregatorFindEventsUtestFreeFilterRules(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->filterRules);
}

static safuResultE_t _backendEmptyFunc(UNUSED elosStorageBackend_t *backend) {
    return SAFU_RESULT_OK;
}

static safuResultE_t _backendPersistFunc(UNUSED elosStorageBackend_t *backend, UNUSED const elosEvent_t *event) {
    return SAFU_RESULT_OK;
}

static safuResultE_t _backendFindFunc(elosStorageBackend_t *backend, elosRpnFilter_t *filter, safuVec_t *events) {
    elosRpnFilterResultE_t filterResult;
    safuResultE_t result;
    elosEvent_t event = {0};

    result = elosEventDeserialize(&event, backend->backendData);
    assert_int_equal(result, SAFU_RESULT_OK);

    filterResult = elosEventFilterExecute(filter, NULL, &event);
    if (filterResult == RPNFILTER_RESULT_MATCH) {
        int retVal;

        retVal = safuVecPush(events, &event);
        assert_int_equal(retVal, 0);
    } else {
        elosEventDeleteMembers(&event);
    }

    return SAFU_RESULT_OK;
}

void elosLogAggregatorFindEventsUtestCreateLogAggregator(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result;
    elosPluginControl_t *plugin = NULL;
    elosPluginControlParam_t pluginParam = {.id = 42};
    elosStorageBackend_t dummyBackend = {
        .name = "utBackend",
        .backendData = testState->eventJsonString,
        .findEvent = _backendFindFunc,
        .persist = _backendPersistFunc,
        .shutdown = _backendEmptyFunc,
        .start = _backendEmptyFunc,
    };
    int retVal;

    result = elosPluginControlNew(&plugin, &pluginParam);
    assert_int_equal(result, SAFU_RESULT_OK);
    atomic_fetch_or(&plugin->flags, (SAFU_FLAG_INITIALIZED_BIT | ELOS_PLUGINCONTROL_FLAG_STARTED_BIT));
    plugin->context.data = safuAllocMem(NULL, sizeof(elosStorageBackend_t));
    memcpy(plugin->context.data, &dummyBackend, sizeof(elosStorageBackend_t));

    testState->logAggregator.lock = safuAllocMem(NULL, sizeof(pthread_mutex_t));
    memset(testState->logAggregator.lock, 0, sizeof(pthread_mutex_t));
    retVal = pthread_mutex_init(testState->logAggregator.lock, NULL);
    assert_int_equal(retVal, 0);

    safuVecCreate(&testState->logAggregator.pluginControlPtrVector, 1, sizeof(elosPluginControl_t *));
    safuVecPush(&testState->logAggregator.pluginControlPtrVector, &plugin);
}

void elosLogAggregatorFindEventsUtestFreeLogAggregator(void **state) {
    elosUteststateT_t *testState = *state;
    elosPluginControl_t *plugin;

    plugin = *(elosPluginControl_t **)safuVecGetLast(&testState->logAggregator.pluginControlPtrVector);
    assert_non_null(plugin);
    free(plugin->context.data);
    free(plugin);
    safuVecFree(&testState->logAggregator.pluginControlPtrVector);

    elosLogAggregatorShutdown(&testState->logAggregator);
    pthread_mutex_destroy(testState->logAggregator.lock);
    free(testState->logAggregator.lock);
}

void elosLogAggregatorFindEventsUtestCreateVector(void **state) {
    elosUteststateT_t *testState = *state;
    memset(&testState->eventVector, 0, sizeof(safuVec_t));
    int res = safuVecCreate(&testState->eventVector, NUM_FILTER, sizeof(elosEvent_t));
    assert_int_equal(res, 0);
}

void elosLogAggregatorFindEventsUtestFreeVector(void **state) {
    elosUteststateT_t *testState = *state;

    for (uint32_t idx = 0; idx < testState->eventVector.elementCount; idx++) {
        elosEvent_t *eventEntry = (elosEvent_t *)safuVecGet(&testState->eventVector, idx);
        assert_non_null(eventEntry);
        elosEventDeleteMembers(eventEntry);
    }

    safuVecFree(&testState->eventVector);
}

static int elosLogAggregatorFindEventsUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLogAggregatorFindEventsUtestTeardown(UNUSED void **state) {
    return 0;
}
