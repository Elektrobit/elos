// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerProcess_utest.h"

int elosTestEloEventFilterManagerProcessErrEfnvSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterManagerProcessErrEfnvTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterManagerProcessErrEfnv(UNUSED void **state) {
    elosEventFilterManager_t efm = {.flags = SAFU_FLAG_INITIALIZED_BIT};
    elosEvent_t const event = {0};
    safuResultE_t result;

    TEST("elosEventFilterManagerProcess");
    SHOULD("%s", "return SAFU_RESULT_FAILED after an failed elosEventFilterNodeVectorProcess call");

    MOCK_FUNC_AFTER_CALL(elosEventFilterNodeVectorProcess, 0);
    expect_value(elosEventFilterNodeVectorProcess, eventFilterNodeVector, &efm.eventFilterNodeVector);
    expect_value(elosEventFilterNodeVectorProcess, filterStack, NULL);
    expect_value(elosEventFilterNodeVectorProcess, event, &event);
    will_return(elosEventFilterNodeVectorProcess, SAFU_RESULT_FAILED);

    result = elosEventFilterManagerProcess(&efm, &event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
