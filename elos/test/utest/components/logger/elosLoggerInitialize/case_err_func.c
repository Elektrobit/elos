// SPDX-License-Identifier: MIT

#include "elosLoggerInitialize_utest.h"
#include "mock_eventbuffer.h"

int elosTestElosLoggerInitializeErrFuncSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLoggerInitializeErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosLoggerDeleteMembers(&test->logger);
    return 0;
}

void elosTestElosLoggerInitializeErrFunc(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosLoggerInitialize");
    SHOULD("%s", "not initialize logger successfully");

    MOCK_FUNC_AFTER_CALL(elosEventBufferNew, 0);
    expect_value(elosEventBufferNew, eventBufferPtr, &test->logger.logEventBuffer);
    expect_not_value(elosEventBufferNew, param, NULL);
    will_return(elosEventBufferNew, SAFU_RESULT_FAILED);

    result = elosLoggerInitialize(&test->logger);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->logger.flags), false);
}
