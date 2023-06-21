// SPDX-License-Identifier: MIT

#include "elosEventQueueManagerEntryNew_utest.h"

#define _EVENTQUEUESIZE 16
#define _LIMITEVENTS    8
#define _LIMITTIME_SEC  1
#define _LIMITTIME_NSEC 2

int elosTestEloEventQueueManagerEntryNewErrPushByParamSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    elosEventQueueManagerParam_t param = {.defaultEventQueueSize = _EVENTQUEUESIZE,
                                          .defaultLimitEvents = _LIMITEVENTS,
                                          .defaultLimitTime = {.tv_sec = _LIMITTIME_SEC, .tv_nsec = _LIMITTIME_NSEC}};

    elosEventQueueManagerInitialize(&test->eqm, &param);
    *state = test;

    return 0;
}

int elosTestEloEventQueueManagerEntryNewErrPushByParamTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventQueueManagerEntryNewErrPushByParam(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;
    elosEventQueueId_t id = ELOS_EVENTQUEUE_ID_INVALID;

    TEST("elosEventQueueManagerEntryNew");
    SHOULD("%s", "test correct behaviour when elosEventQueueVectorPushByParam() fails");
    SKIP("%s", "Due to eventprocessor_static not working together with mock_eventprocessor");

    MOCK_FUNC_AFTER_CALL(elosEventQueueVectorPushByParam, 0);
    expect_not_value(elosEventQueueVectorPushByParam, eventQueueVector, NULL);
    expect_not_value(elosEventQueueVectorPushByParam, param, NULL);
    will_return(elosEventQueueVectorPushByParam, SAFU_RESULT_FAILED);

    result = elosEventQueueManagerEntryNew(&test->eqm, &id);
    assert_true(result == SAFU_RESULT_FAILED);
}
