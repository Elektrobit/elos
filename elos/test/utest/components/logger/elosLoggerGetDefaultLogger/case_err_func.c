// SPDX-License-Identifier: MIT

#include "elosLoggerGetDefaultLogger_utest.h"
#include "mock_logger.h"

int elosTestElosLoggerGetDefaultLoggerErrFuncSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLoggerGetDefaultLoggerErrFuncTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLoggerGetDefaultLoggerErrFunc(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosLogger_t *testLogger = NULL;
    safuResultE_t result;

    TEST("elosLoggerGetDefaultLogger");
    SHOULD("%s", "fail to return Default logger");

    testLogger = &test->logger;

    MOCK_FUNC_AFTER_CALL(elosLoggerInitialize, 0);
    expect_any(elosLoggerInitialize, logger);
    will_return(elosLoggerInitialize, SAFU_RESULT_FAILED);

    result = elosLoggerGetDefaultLogger(&testLogger);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&testLogger->flags), false);
}
