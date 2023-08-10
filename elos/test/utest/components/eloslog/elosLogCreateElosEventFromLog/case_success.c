// SPDX-License-Identifier: MIT

#include "elosLogCreateElosEventFromLog_utest.h"

int elosLogCreateElosEventFromLogSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosLogCreateElosEventFromLogSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosLogCreateElosEventFromLogSuccess(UNUSED void **state) {
    elosLogStatusE_t result = ELOS_LOG_STATUS_ERROR;
    elosEvent_t *testEvent = NULL;
    elosLogData_t *testLog = NULL;
    const char *testMessage = "testMessage";

    elosLogCreateLogData(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, testMessage,
                         &testLog);

    result = elosLogCreateElosEventFromLog(testLog, &testEvent);

    assert_int_equal(result, ELOS_LOG_STATUS_SUCCESS);
    assert_int_equal(testEvent->messageCode, ELOS_MSG_CODE_DEBUG_LOG);
    assert_int_equal(testEvent->severity, ELOS_SEVERITY_DEBUG);
    assert_int_equal(testEvent->classification, ELOS_CLASSIFICATION_PROCESS);
    assert_string_equal(testEvent->payload, testMessage);

    elosLogDeleteLogData(testLog);
    elosEventDelete(testEvent);
}
