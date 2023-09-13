// SPDX-License-Identifier: MIT

#include "elosLog_utest.h"
#include "mock_logger.h"

int elosTestElosLogSuccessSetup(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosLoggerInitialize(&test->logger);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->logger.flags), true);

    return 0;
}

int elosTestElosLogSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosLoggerDeleteMembers(&test->logger);
    return 0;
}

void elosTestElosLogSuccess(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    const char *testString = "logger test";

    TEST("elosLog");
    SHOULD("%s", "log event successfully");

    MOCK_FUNC_AFTER_CALL(elosLoggerGetDefaultLogger, 0)
    expect_any(elosLoggerGetDefaultLogger, logger);
    will_set_parameter(elosLoggerGetDefaultLogger, logger, &test->logger);
    will_return(elosLoggerGetDefaultLogger, SAFU_RESULT_OK);

    elosLog(ELOS_MSG_CODE_UNAUTHORIZED_PUBLISHING, ELOS_SEVERITY_WARN,
            ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS | ELOS_CLASSIFICATION_IPC, testString);
}
