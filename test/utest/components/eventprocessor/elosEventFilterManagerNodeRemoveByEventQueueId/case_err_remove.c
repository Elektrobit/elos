// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerNodeRemove_utest.h"
#include "mock_eventfilternodevector.h"
#include "safu/mock_log.h"

int elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrRemoveSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    elosEventFilterManagerParam_t param = {
        .eventQueueManager = &test->eventQueueManager,
        .idManager = NULL,
        .elements = 8,
    };

    result = elosEventFilterManagerInitialize(&test->eventFilterManager, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrRemoveTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    assert_int_equal(result, SAFU_RESULT_OK);

    memset(test, 0, sizeof(elosUnitTestState_t));

    return 0;
}

void elosTestEloEventFilterManagerNodeRemoveByEventQueueIdErrRemove(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventQueueId_t eventQueueId = 21;
    safuResultE_t result;

    TEST("elosEventFilterManagerNodeRemove");
    SHOULD("%s", "fail as elosEventFilterNodeVectorRemoveById fails");

    MOCK_FUNC_ENABLE(safuVecFindRemove);
    expect_not_value(__wrap_safuVecFindRemove, vec, NULL);
    expect_not_value(__wrap_safuVecFindRemove, matchFunc, NULL);
    expect_not_value(__wrap_safuVecFindRemove, matchData, NULL);
    will_return(__wrap_safuVecFindRemove, SAFU_RESULT_FAILED);

    result = elosEventFilterManagerNodeRemoveByEventQueueId(&test->eventFilterManager, eventQueueId);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
