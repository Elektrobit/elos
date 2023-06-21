// SPDX-License-Identifier: MIT

#include "elosEventVectorInitialize_utest.h"

int elosTestElosEventVectorInitializeErrEventVectorSetup(void **state) {
    elosUteststateT_t *testState = *state;
    testState->numElements = 3;
    return 0;
}

int elosTestElosEventVectorInitializeErrEventVectorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorInitializeErrEventVector(void **state) {
    elosUteststateT_t *testState = *state;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorInitialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED when eventVector is null");

    result = elosEventVectorInitialize(NULL, testState->numElements);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
