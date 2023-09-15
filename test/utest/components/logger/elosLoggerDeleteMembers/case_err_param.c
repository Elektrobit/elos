// SPDX-License-Identifier: MIT

#include "elosLoggerDeleteMembers_utest.h"
#include "mock_eventbuffer.h"

int elosTestElosLoggerDeleteMembersErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLoggerDeleteMembersErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLoggerDeleteMembersErrParam(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosLogger_t testLogger = {0};
    test->logger = testLogger;
    safuResultE_t result;

    TEST("elosLoggerDeleteMembers");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosLoggerDeleteMembers(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalid logger");
    result = elosLoggerDeleteMembers(&test->logger);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
