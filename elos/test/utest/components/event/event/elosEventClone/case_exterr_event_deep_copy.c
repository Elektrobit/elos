// SPDX-License-Identifier: MIT
#include "elosEventClone_utest.h"

int elosTestElosEventCloneExtErrEventDeepCopySetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventCloneExtErrEventDeepCopyTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventCloneExtErrEventDeepCopy(UNUSED void **state) {
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
    SHOULD("%s", "return SAFU_RESULT_FAILED when elosEventDeepCopy failed");

    MOCK_FUNC_AFTER_CALL(elosEventDeepCopy, 0);
    expect_not_value(elosEventDeepCopy, to, NULL);
    expect_value(elosEventDeepCopy, from, &testSource);
    will_return(elosEventDeepCopy, SAFU_RESULT_FAILED);

    result = elosEventClone(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
