// SPDX-License-Identifier: MIT

#include "elosLogCreateElosEventFromLog_utest.h"

int elosLogCreateElosEventFromLogErrParamEventSetup(UNUSED void **state) {
    return 0;
}

int elosLogCreateElosEventFromLogErrParamEventTeardown(UNUSED void **state) {
    return 0;
}

void elosLogCreateElosEventFromLogErrParamEvent(UNUSED void **state) {
    elosLogStatusE_t result = ELOS_LOG_STATUS_ERROR;
    elosLogData_t *testLog = NULL;
    const char *testMessage = "testMessage";

    elosLogCreateLogData(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, testMessage,
                         &testLog);

    result = elosLogCreateElosEventFromLog(testLog, NULL);

    assert_int_equal(result, ELOS_LOG_STATUS_ERROR);

    elosLogDeleteLogData(testLog);
}
