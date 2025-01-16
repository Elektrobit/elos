// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeProcess_utest.h"
#include "mock_eventfilter.h"
#include "mock_eventqueue.h"
#include "safu/mock_vector.h"

int elosTestEloEventFilterNodeProcessErrIterSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const *filterString[] = {".event.hardwareid 'test123' STRCMP", ".event.messageCode 1000 EQ"};
    uint32_t filterCount = ARRAY_SIZE(filterString);
    elosEventFilterNodeSetup(test, filterString, filterCount);
    return 0;
}

int elosTestEloEventFilterNodeProcessErrIterTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventFilterNodeTeardown(test);
    return 0;
}

void elosTestEloEventFilterNodeProcessErrIter(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t const event = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test234"};
    safuResultE_t result;

    TEST("elosEventFilterNodeProcess (fail iteration)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after failed iteration");

    // safuVecIterate is a wrapper macro around safuVecFind, so we have to mock it here
    PARAM("%s", "fail safuVecIterate");
    MOCK_FUNC_AFTER_CALL(safuVecFind, 0);
    expect_not_value(__wrap_safuVecFind, vec, NULL);
    expect_value(__wrap_safuVecFind, startIdx, 0);
    expect_not_value(__wrap_safuVecFind, matchFunc, 0);
    expect_not_value(__wrap_safuVecFind, matchData, 0);
    expect_value(__wrap_safuVecFind, resultIdx, NULL);
    will_return(__wrap_safuVecFind, -1);

    result = elosEventFilterNodeProcess(&test->eventFilterNode, NULL, &event);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "fail elosEventFilterExecute during iteration");
    MOCK_FUNC_AFTER_CALL(elosEventFilterExecute, 0);
    expect_not_value(elosEventFilterExecute, filter, NULL);
    expect_value(elosEventFilterExecute, param, NULL);
    expect_not_value(elosEventFilterExecute, input, NULL);
    will_return(elosEventFilterExecute, RPNFILTER_RESULT_ERROR);

    result = elosEventFilterNodeProcess(&test->eventFilterNode, NULL, &event);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "fail elosEventQueuePush during iteration");
    MOCK_FUNC_AFTER_CALL(elosEventQueuePush, 0);
    expect_not_value(elosEventQueuePush, eventQueue, NULL);
    expect_not_value(elosEventQueuePush, event, NULL);
    will_return(elosEventQueuePush, SAFU_RESULT_FAILED);

    result = elosEventFilterNodeProcess(&test->eventFilterNode, NULL, &event);
    assert_true(result == SAFU_RESULT_FAILED);
}
