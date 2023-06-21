// SPDX-License-Identifier: MIT

#include <safu/vector.h>

#include "elosEventQueueManagerEntryDelete_utest.h"

#define _EVENTQUEUEID      42
#define _EVENTQUEUESIZE    16
#define _LIMITEVENTS       8
#define _LIMITTIME_SEC     1
#define _LIMITTIME_NSEC    2
#define _EXPECTEDQUEUESIZE 1

int elosTestEloEventQueueManagerEntryDeleteErrWrongIdSetup(void **state) {
    elosTestState_t *test = calloc(1, sizeof(elosTestState_t));
    elosEventQueueManagerParam_t param = {.defaultEventQueueSize = _EVENTQUEUESIZE,
                                          .defaultLimitEvents = _LIMITEVENTS,
                                          .defaultLimitTime = {.tv_sec = _LIMITTIME_SEC, .tv_nsec = _LIMITTIME_NSEC}};

    elosEventQueueManagerInitialize(&test->eqm, &param);
    elosEventQueueManagerEntryNew(&test->eqm, &test->id);

    *state = test;

    return 0;
}

int elosTestEloEventQueueManagerEntryDeleteErrWrongIdTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventQueueManagerDeleteMembers(&test->eqm);
    free(test);

    return 0;
}

void elosTestEloEventQueueManagerEntryDeleteErrWrongId(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;
    elosEventQueueId_t wrongId = test->id + 1;

    TEST("elosEventQueueManagerEntryDelete");
    SHOULD("%s", "test correct behaviour of elosEventQueueManagerEntryDelete");

    assert_true(test->id != ELOS_EVENTQUEUE_ID_INVALID);
    result = elosEventQueueManagerEntryDelete(&test->eqm, wrongId);
    assert_true(result == SAFU_RESULT_FAILED);
    assert_true(safuVecElements(&test->eqm.eventQueueVector) == _EXPECTEDQUEUESIZE);
}
