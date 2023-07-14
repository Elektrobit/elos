// SPDX-License-Identifier: MIT
#include "elosEventClone_utest.h"

int elosTestElosEventCloneExtErrEventNewSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventCloneExtErrEventNewTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventCloneExtErrEventNew(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    const elosEvent_t testSource = {
        .date = {.tv_sec = 25, .tv_nsec = 100},
        .source = {0},
        .severity = ELOS_SEVERITY_DEBUG,
        .hardwareid = "localhost",
        .classification = 42,
        .messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION,
        .payload = "payload",
    };
    elosEvent_t *testDest = NULL;

    TEST("elosEventClone");
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventNew failed");

    MOCK_FUNC_AFTER_CALL(elosEventNew, 0);
    expect_not_value(elosEventNew, event, NULL);
    will_return(elosEventNew, SAFU_RESULT_FAILED);

    result = elosEventClone(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
