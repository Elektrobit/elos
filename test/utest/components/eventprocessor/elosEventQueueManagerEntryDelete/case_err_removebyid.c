// SPDX-License-Identifier: MIT

#include "elosEventQueueManagerEntryDelete_utest.h"

#define _EVENTQUEUESIZE 16
#define _LIMITEVENTS    8
#define _LIMITTIME_SEC  1
#define _LIMITTIME_NSEC 2

int elosTestEloEventQueueManagerEntryDeleteErrPushByParamSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    elosEventQueueManagerParam_t param = {.defaultEventQueueSize = _EVENTQUEUESIZE,
                                          .defaultLimitEvents = _LIMITEVENTS,
                                          .defaultLimitTime = {.tv_sec = _LIMITTIME_SEC, .tv_nsec = _LIMITTIME_NSEC}};

    elosEventQueueManagerInitialize(&test->eqm, &param);
    *state = test;

    return 0;
}

int elosTestEloEventQueueManagerEntryDeleteErrPushByParamTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventQueueManagerEntryDeleteErrPushByParam(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;
    elosEventQueueId_t id = 42;

    TEST("elosEventQueueManagerEntryDelete");
    SHOULD("%s", "test correct behaviour when safuVecFree() fails");
    SKIP("elosEventQueueVector mocking not implemented yet");

    // .. mocking

    result = elosEventQueueManagerEntryDelete(&test->eqm, id);
    assert_true(result == SAFU_RESULT_FAILED);
}
