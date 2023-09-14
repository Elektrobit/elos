// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerDeleteMembers_utest.h"
#include "mock_eventfilternodevector.h"
#include "safu/mock_log.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterManagerDeleteMembersRemovalFailedSetup(void **state) {
    elosTestState_t *test;
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosTestState_t));
    assert_non_null(test);

    memset(test, 0, sizeof(elosTestState_t));

    elosEventFilterManagerParam_t param = {
        .elements = 3,
        .eventQueueManager = &test->eventQueueManager,
        .idManager = &test->idManager,
    };
    result = elosEventFilterManagerInitialize(&test->eventFilterManager, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterManagerDeleteMembersRemovalFailedTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;

    elosEventFilterNodeVectorDeleteMembers(&test->eventFilterManager.eventFilterNodeVector);
    elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    free(*state);
    *state = NULL;

    return 0;
}

void elosTestEloEventFilterManagerDeleteMembersRemovalFailed(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventFilterManagerDeleteMembers");
    SHOULD("%s", "fail as elosEventFilterNodeVectorDeleteMembers failed");

    MOCK_FUNC_ENABLE(elosEventFilterNodeVectorDeleteMembers);
    expect_value(elosEventFilterNodeVectorDeleteMembers, eventFilterNodeVector,
                 &test->eventFilterManager.eventFilterNodeVector);
    will_return(elosEventFilterNodeVectorDeleteMembers, SAFU_RESULT_FAILED);

    result = elosEventFilterManagerDeleteMembers(&test->eventFilterManager);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
