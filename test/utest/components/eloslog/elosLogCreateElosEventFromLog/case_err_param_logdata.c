// SPDX-License-Identifier: MIT

#include "elosLogCreateElosEventFromLog_utest.h"

int elosLogCreateElosEventFromLogErrParamLogDataSetup(UNUSED void **state) {
    return 0;
}

int elosLogCreateElosEventFromLogErrParamLogDataTeardown(UNUSED void **state) {
    return 0;
}

void elosLogCreateElosEventFromLogErrParamLogData(UNUSED void **state) {
    elosLogStatusE_t result = ELOS_LOG_STATUS_ERROR;
    elosEvent_t *testEvent = NULL;

    TEST("elosLogCreateElosEventFromLog");
    SHOULD("%s", "not create an elos event as input param is NULL");

    result = elosLogCreateElosEventFromLog(NULL, &testEvent);

    assert_int_equal(result, ELOS_LOG_STATUS_ERROR);
    assert_null(testEvent);
}
