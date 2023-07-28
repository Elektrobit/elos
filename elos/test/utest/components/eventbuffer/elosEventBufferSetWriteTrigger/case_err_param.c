// SPDX-License-Identifier: MIT

#include "elosEventBufferSetWriteTrigger_utest.h"

int elosTestElosEventBufferSetWriteTriggerErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventBufferSetWriteTriggerErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventBufferSetWriteTriggerErrParam(UNUSED void **state) {
    elosEventBuffer_t invalidEventBuffer = {0};
    safuResultE_t result;

    TEST("elosEventBufferSetWriteTrigger");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosEventBufferSetWriteTrigger(NULL, 0);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "invalidEventBuffer");
    result = elosEventBufferSetWriteTrigger(&invalidEventBuffer, 0);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
