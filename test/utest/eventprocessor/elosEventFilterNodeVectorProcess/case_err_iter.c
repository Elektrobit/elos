// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorProcess_utest.h"
#include "mock_eventfilternode.h"
#include "safu/mock_vector.h"

int elosTestEloEventFilterNodeVectorProcessErrIterSetup(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    char const fStrA[] = {".event.messageCode 1000 EQ"};
    char const fStrB[] = {".event.hardwareid 'test123' STRCMP"};

    memset(test, 0, sizeof(elosTestState_t));
    elosEventFilterNodeVectorSetup(test, fStrA, fStrB);

    return 0;
}

int elosTestEloEventFilterNodeVectorProcessErrIterTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventFilterNodeVectorTeardown(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorProcessErrIter(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEvent_t const eventA = {.messageCode = ELOS_MSG_CODE_WILDCARD_1XXX, .hardwareid = "test234"};
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorProcess (fail iteration)");
    SHOULD("%s", "return SAFU_RESULT_FAILED after failed element iteration");

    // safuVecIterate is a wrapper macro around safuVecFind, so we have to mock it here
    PARAM("%s", "fail safuVecIterate");
    MOCK_FUNC_AFTER_CALL(safuVecFind, 0);
    expect_not_value(__wrap_safuVecFind, vec, NULL);
    expect_value(__wrap_safuVecFind, startIdx, 0);
    expect_not_value(__wrap_safuVecFind, matchFunc, 0);
    expect_not_value(__wrap_safuVecFind, matchData, 0);
    expect_value(__wrap_safuVecFind, resultIdx, NULL);
    will_return(__wrap_safuVecFind, -1);

    result = elosEventFilterNodeVectorProcess(&test->eventFilterNodeVector, NULL, &eventA);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "fail elosEventFilterNodeProcess during iteration");
    MOCK_FUNC_AFTER_CALL(elosEventFilterNodeProcess, 0);
    expect_not_value(elosEventFilterNodeProcess, eventFilterNode, NULL);
    expect_value(elosEventFilterNodeProcess, filterStack, NULL);
    expect_value(elosEventFilterNodeProcess, event, &eventA);
    will_return(elosEventFilterNodeProcess, SAFU_RESULT_FAILED);

    result = elosEventFilterNodeVectorProcess(&test->eventFilterNodeVector, NULL, &eventA);
    assert_true(result == SAFU_RESULT_FAILED);
}
