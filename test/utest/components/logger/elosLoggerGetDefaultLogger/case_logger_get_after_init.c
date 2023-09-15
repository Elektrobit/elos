// SPDX-License-Identifier: MIT

#include "elosLoggerGetDefaultLogger_utest.h"

int elosTestElosLoggerGetDefaultLoggerGetAfterLoggerInitSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosLoggerInitialize(&test->logger);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->logger.flags), true);

    return 0;
}

int elosTestElosLoggerGetDefaultLoggerGetAfterLoggerInitTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLoggerGetDefaultLoggerGetAfterLoggerInit(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosLogger_t *testLogger = NULL;
    safuResultE_t result;

    TEST("elosLoggerGetDefaultLogger");
    SHOULD("%s", "return default logger which is already initialized");

    testLogger = &test->logger;

    result = elosLoggerGetDefaultLogger(&testLogger);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&testLogger->flags), true);

    elosLoggerDeleteMembers(&test->logger);
}
