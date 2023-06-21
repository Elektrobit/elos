// SPDX-License-Identifier: MIT

#include "elosEventQueueManagerEntryDelete_utest.h"

int elosTestEloEventQueueManagerEntryDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventQueueManagerEntryDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventQueueManagerEntryDeleteErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventQueueManager_t eqm = {0};
    elosEventQueueId_t id = 42;

    TEST("elosEventQueueManagerEntryDelete");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    result = elosEventQueueManagerEntryDelete(NULL, ELOS_EVENTQUEUE_ID_INVALID);
    assert_true(result == SAFU_RESULT_FAILED);

    result = elosEventQueueManagerEntryDelete(&eqm, ELOS_EVENTQUEUE_ID_INVALID);
    assert_true(result == SAFU_RESULT_FAILED);

    result = elosEventQueueManagerEntryDelete(NULL, id);
    assert_true(result == SAFU_RESULT_FAILED);
}
