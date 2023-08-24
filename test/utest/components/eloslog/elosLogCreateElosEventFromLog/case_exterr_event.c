// SPDX-License-Identifier: MIT

#include "elosLogCreateElosEventFromLog_utest.h"

int elosLogCreateElosEventFromLogExtErrEventSetup(UNUSED void **state) {
    return 0;
}

int elosLogCreateElosEventFromLogExtErrEventTeardown(UNUSED void **state) {
    return 0;
}

void elosLogCreateElosEventFromLogExtErrEvent(UNUSED void **state) {
    elosLogStatusE_t result = ELOS_LOG_STATUS_ERROR;
    elosEvent_t *testEvent = NULL;
    elosLogData_t *testLog = NULL;
    const char *testMessage = "testMessage";

    TEST("elosLogCreateElosEventFromLog");
    SHOULD("%s", "elos event creation fail as new event is not created");

    elosLogCreateLogData(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, testMessage,
                         &testLog);
    assert_non_null(testLog);

    MOCK_FUNC_AFTER_CALL(elosEventNew, 0);
    expect_not_value(elosEventNew, event, NULL);
    will_return(elosEventNew, SAFU_RESULT_FAILED);

    result = elosLogCreateElosEventFromLog(testLog, &testEvent);

    assert_int_equal(result, ELOS_LOG_STATUS_ERROR);
    assert_null(testEvent);

    elosLogDeleteLogData(testLog);
}
