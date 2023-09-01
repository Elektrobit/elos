// SPDX-License-Identifier: MIT

#include "elosLoggerInitialize_utest.h"

int elosTestElosLoggerInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLoggerInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosLoggerDeleteMembers(&test->logger);
    return 0;
}

void elosTestElosLoggerInitializeSuccess(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosLoggerInitialize");
    SHOULD("%s", "initialize logger successfully");

    result = elosLoggerInitialize(&test->logger);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->logger.flags), true);
    assert_int_equal(test->logger.logEventBuffer->flags, true);
    assert_int_equal(test->logger.logEventBuffer->ringCount, ELOS_EVENTBUFFER_PRIORITY_COUNT);
}
