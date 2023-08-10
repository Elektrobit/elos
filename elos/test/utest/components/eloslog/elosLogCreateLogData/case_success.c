// SPDX-License-Identifier: MIT

#include "elosLogCreateLogData_utest.h"

int elosLogCreateLogDataSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosLogCreateLogDataSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosLogCreateLogDataSuccess(UNUSED void **state) {
    elosLogData_t *testLog = NULL;
    const char *testMessage = "testMessage";

    elosLogCreateLogData(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, testMessage,
                         &testLog);

    assert_int_equal(testLog->messageCode, ELOS_MSG_CODE_DEBUG_LOG);
    assert_int_equal(testLog->severity, ELOS_SEVERITY_DEBUG);
    assert_int_equal(testLog->classification, ELOS_CLASSIFICATION_PROCESS);
    assert_string_equal(testLog->payload, testMessage);

    elosLogDeleteLogData(testLog);
}
