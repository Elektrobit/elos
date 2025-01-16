// SPDX-License-Identifier: MIT

#include "elosEventQueueManagerEntryNew_utest.h"

#define _EVENTQUEUEID   1
#define _EVENTQUEUESIZE 16
#define _LIMITEVENTS    8
#define _LIMITTIME_SEC  1
#define _LIMITTIME_NSEC 2

int elosTestEloEventQueueManagerEntryNewSuccessSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    elosEventQueueManagerParam_t param = {.defaultEventQueueSize = _EVENTQUEUESIZE,
                                          .defaultLimitEvents = _LIMITEVENTS,
                                          .defaultLimitTime = {.tv_sec = _LIMITTIME_SEC, .tv_nsec = _LIMITTIME_NSEC}};

    elosEventQueueManagerInitialize(&test->eqm, &param);
    *state = test;

    return 0;
}

int elosTestEloEventQueueManagerEntryNewSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventQueueManagerEntryNewSuccess(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;
    elosEventQueueId_t id = ELOS_EVENTQUEUE_ID_INVALID;

    TEST("elosEventQueueManagerEntryNew");
    SHOULD("%s", "test correct behaviour of elosEventQueueManagerEntryNew");

    result = elosEventQueueManagerEntryNew(&test->eqm, &id);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(id == _EVENTQUEUEID);
    assert_true(safuVecElements(&test->eqm.eventQueueVector) == 1);

    result = elosEventQueueManagerEntryNew(&test->eqm, &id);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(id == (_EVENTQUEUEID + 1));
    assert_true(safuVecElements(&test->eqm.eventQueueVector) == 2);
}
