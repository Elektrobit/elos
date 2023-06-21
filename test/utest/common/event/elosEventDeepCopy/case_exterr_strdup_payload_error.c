// SPDX-License-Identifier: MIT
#include "elosEventDeepCopy_utest.h"

int elosTestElosEventStrdupPayloadErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventStrdupPayloadErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventStrdupPayloadError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    const elosEvent_t testSource = {.payload = "payload"};
    elosEvent_t testDest = {0};

    TEST("elosEventDeepCopy");
    SHOULD("%s", "return error since copying payload fails ");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_string(__wrap_strdup, string, testSource.payload);
    will_return(__wrap_strdup, NULL);

    result = elosEventDeepCopy(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
