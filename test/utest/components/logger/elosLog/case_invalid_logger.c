// SPDX-License-Identifier: MIT

#include "elosLog_utest.h"
#include "mock_logger.h"

int elosTestElosLogInvalidLoggerSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLogInvalidLoggerTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLogInvalidLogger(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosLogger_t testLogger = {0};
    test->logger = testLogger;
    const char *testString = "logger test";

    TEST("elosLog");
    SHOULD("%s", "not log event as logger is invalid");

    MOCK_FUNC_AFTER_CALL(elosLoggerGetDefaultLogger, 0)
    expect_any(elosLoggerGetDefaultLogger, logger);
    will_set_parameter(elosLoggerGetDefaultLogger, logger, &test->logger);
    will_return(elosLoggerGetDefaultLogger, SAFU_RESULT_OK);

    elosLog(ELOS_MSG_CODE_UNAUTHORIZED_PUBLISHING, ELOS_SEVERITY_WARN,
            ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS | ELOS_CLASSIFICATION_IPC, testString);
}
