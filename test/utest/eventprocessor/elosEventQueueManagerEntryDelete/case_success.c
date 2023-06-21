// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosEventQueueManagerEntryDelete_utest.h"

#define _EVENTQUEUEID   42
#define _EVENTQUEUESIZE 16
#define _LIMITEVENTS    8
#define _LIMITTIME_SEC  1
#define _LIMITTIME_NSEC 2

int elosTestEloEventQueueManagerEntryDeleteSuccessSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    elosEventQueueManagerParam_t param = {.defaultEventQueueSize = _EVENTQUEUESIZE,
                                          .defaultLimitEvents = _LIMITEVENTS,
                                          .defaultLimitTime = {.tv_sec = _LIMITTIME_SEC, .tv_nsec = _LIMITTIME_NSEC}};

    elosEventQueueManagerInitialize(&test->eqm, &param);
    elosEventQueueManagerEntryNew(&test->eqm, &test->id);
    elosEventQueueManagerEntryNew(&test->eqm, &test->id);

    *state = test;

    return 0;
}

int elosTestEloEventQueueManagerEntryDeleteSuccessTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventQueueManagerEntryDeleteSuccess(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventQueueManagerEntryDelete");
    SHOULD("%s", "test correct behaviour of elosEventQueueManagerEntryDelete");

    assert_true(safuVecElements(&test->eqm.eventQueueVector) == 2);

    result = elosEventQueueManagerEntryDelete(&test->eqm, test->id);
    assert_true(result == SAFU_RESULT_OK);
    assert_true(safuVecElements(&test->eqm.eventQueueVector) == 1);

    result = elosEventQueueManagerEntryDelete(&test->eqm, (test->id - 1));
    assert_true(result == SAFU_RESULT_OK);
    assert_true(safuVecElements(&test->eqm.eventQueueVector) == 0);
}
