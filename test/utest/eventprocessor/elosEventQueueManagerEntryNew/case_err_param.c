// SPDX-License-Identifier: MIT

#include "elosEventQueueManagerEntryNew_utest.h"

int elosTestEloEventQueueManagerEntryNewErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventQueueManagerEntryNewErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventQueueManagerEntryNewErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventQueueManager_t eqm = {0};
    elosEventQueueId_t id = 0;

    TEST("elosEventQueueManagerEntryNew");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    result = elosEventQueueManagerEntryNew(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    result = elosEventQueueManagerEntryNew(&eqm, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    result = elosEventQueueManagerEntryNew(NULL, &id);
    assert_true(result == SAFU_RESULT_FAILED);
}
