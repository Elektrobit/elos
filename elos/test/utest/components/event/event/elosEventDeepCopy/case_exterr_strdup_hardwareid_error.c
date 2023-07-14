// SPDX-License-Identifier: MIT
#include "elosEventDeepCopy_utest.h"

int elosTestElosEventStrdupHardwareIdErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventStrdupHardwareIdErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventStrdupHardwareIdError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    const elosEvent_t testSource = {.hardwareid = "localhost"};
    elosEvent_t testDest = {0};

    TEST("elosEventDeepCopy");
    SHOULD("%s", "return error since copying hardwareid fails ");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_string(__wrap_strdup, string, testSource.hardwareid);
    will_return(__wrap_strdup, NULL);

    result = elosEventDeepCopy(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
