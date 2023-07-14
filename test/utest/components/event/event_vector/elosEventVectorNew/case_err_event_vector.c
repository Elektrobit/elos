// SPDX-License-Identifier: MIT

#include "elosEventVectorNew_utest.h"

int elosTestElosEventVectorNewErrEventVectorSetup(void **state) {
    elosUteststateT_t *testState = *state;
    testState->numElements = 3;
    return 0;
}

int elosTestElosEventVectorNewErrEventVectorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorNewErrEventVector(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorNew");
    SHOULD("%s", "return SAFU_RESULT_FAILED when eventVector is null");

    result = elosEventVectorNew(NULL, testState->numElements);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
