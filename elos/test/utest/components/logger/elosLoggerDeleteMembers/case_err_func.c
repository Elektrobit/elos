// SPDX-License-Identifier: MIT

#include "elosLoggerDeleteMembers_utest.h"
#include "mock_eventbuffer.h"

int elosTestElosLoggerDeleteMembersErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosLoggerInitialize(&test->logger);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->logger.flags), true);

    return 0;
}

int elosTestElosLoggerDeleteMembersErrFuncTeardown(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosLoggerDeleteMembers(&test->logger);
    return 0;
}

void elosTestElosLoggerDeleteMembersErrFunc(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosLoggerDeleteMembers");
    SHOULD("%s", "not initialize logger successfully");

    MOCK_FUNC_AFTER_CALL(elosEventBufferDelete, 0);
    expect_value(elosEventBufferDelete, eventBufferPtr, &test->logger.logEventBuffer);
    will_return(elosEventBufferDelete, SAFU_RESULT_FAILED);

    result = elosLoggerDeleteMembers(&test->logger);

    assert_int_equal(result, SAFU_RESULT_FAILED);

    MOCK_FUNC_DISABLE(elosEventBufferDelete);
}
