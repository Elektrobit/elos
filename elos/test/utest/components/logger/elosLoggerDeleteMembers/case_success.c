// SPDX-License-Identifier: MIT

#include "elosLoggerDeleteMembers_utest.h"

int elosTestElosLoggerDeleteMembersSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    result = elosLoggerInitialize(&test->logger);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->logger.flags), true);

    return 0;
}

int elosTestElosLoggerDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLoggerDeleteMembersSuccess(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosLoggerDeleteMembers");
    SHOULD("%s", "delete logger members successfully");

    result = elosLoggerDeleteMembers(&test->logger);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->logger.flags), false);
    assert_null(test->logger.logEventBuffer);
}
