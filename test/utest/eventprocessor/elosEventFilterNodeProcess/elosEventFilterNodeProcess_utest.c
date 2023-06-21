// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeProcess_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeProcessErrIter),
        TEST_CASE(elosTestEloEventFilterNodeProcessErrParam),
        TEST_CASE(elosTestEloEventFilterNodeProcessSuccess),
        TEST_CASE(elosTestEloEventFilterNodeProcessSuccessWithVStack),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosTestState_t));
    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    return 0;
}

void elosEventFilterNodeSetup(elosTestState_t *test, char const **filterString, uint32_t filterCount) {
    elosEventQueueParam_t const eventQueueParam = _EVENTQUEUE_PARAM;
    elosEventFilterNodeParam_t eventFilterNodeParam = {
        .id = 84, .filterStrings = filterString, .filterStringCount = filterCount, .eventQueue = NULL};
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));
    result = elosEventQueueNew(&eventFilterNodeParam.eventQueue, &eventQueueParam);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventFilterNodeInitialize(&test->eventFilterNode, &eventFilterNodeParam);
    assert_true(result == SAFU_RESULT_OK);
    assert_int_equal(test->eventFilterNode.eventFilterVector.elementCount, 2);
}

void elosEventFilterNodeTeardown(elosTestState_t *test) {
    elosEventQueueDelete(test->eventFilterNode.eventQueue);
    elosEventFilterNodeDeleteMembers(&test->eventFilterNode);
}

void elosEventFilterNodeTestStep(elosTestState_t *test, char const *message, elosEvent_t const *event,
                                 elosEventFilterStack_t const *vStack, uint32_t queueElements, bool matchLastEvent) {
    elosEvent_t *testEvent;
    safuResultE_t result;

    PARAM("%s", message);
    result = elosEventFilterNodeProcess(&test->eventFilterNode, vStack, event);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(test->eventFilterNode.eventQueue->eventVector->elementCount == queueElements);
    testEvent = (elosEvent_t *)safuVecGetLast(test->eventFilterNode.eventQueue->eventVector);
    assert_non_null(testEvent);

    if (matchLastEvent == true) {
        assert_string_equal(testEvent->hardwareid, event->hardwareid);
        assert_int_equal(testEvent->messageCode, event->messageCode);
    } else {
        assert_string_not_equal(testEvent->hardwareid, event->hardwareid);
        assert_int_not_equal(testEvent->messageCode, event->messageCode);
    }
}
