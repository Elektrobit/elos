// SPDX-License-Identifier: MIT

#include "elosLogDeleteLogData_utest.h"

int elosLogDeleteLogDataSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosLogDeleteLogDataSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosLogDeleteLogDataSuccess(UNUSED void **state) {
    elosLogData_t *testLog = NULL;
    const char *testMessage = "testMessage";

    assert_null(testLog);
    elosLogCreateLogData(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, testMessage,
                         &testLog);

    elosLogDeleteLogData(testLog);

    assert_non_null(testLog);
}
