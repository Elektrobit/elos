// SPDX-License-Identifier: MIT

#include "elosEventBufferDelete_utest.h"

int elosTestElosEventBufferDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventBufferDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventBufferDeleteErrParam(UNUSED void **state) {
    elosEventBuffer_t invalidEventBuffer = {0};
    elosEventBuffer_t *invalidEventBufferPtr = &invalidEventBuffer;
    safuResultE_t result;

    TEST("elosEventBufferDelete");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventBufferDelete(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventBuffer");
    result = elosEventBufferDelete(&invalidEventBufferPtr);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
