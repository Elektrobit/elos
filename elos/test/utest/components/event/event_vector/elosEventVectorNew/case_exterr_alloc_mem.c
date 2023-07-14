// SPDX-License-Identifier: MIT

#include "elosEventVectorNew_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventVectorExtErrAllocMemSetup(void **state) {
    elosUteststateT_t *testState = *state;
    testState->numElements = 3;
    return 0;
}

int elosTestElosEventVectorExtErrAllocMemTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorExtErrAllocMem(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorNew");
    SHOULD("%s", "return SAFU_RESULT_FAILED when safuAllocMem failed");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosEventVector_t));
    will_return(__wrap_safuAllocMem, NULL);

    result = elosEventVectorNew(&testState->eventVector, testState->numElements);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
