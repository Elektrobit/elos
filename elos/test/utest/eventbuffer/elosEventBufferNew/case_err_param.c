// SPDX-License-Identifier: MIT

#include "elosEventBufferNew_utest.h"

#define _TEST_EVENT_LIMIT_COUNT 8

int elosTestElosEventBufferNewErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventBufferNewErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventBufferNewErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventBuffer_t *eventBufferPtr = NULL;
    elosEventBufferParam_t const param = {
        .limitEventCount = _TEST_EVENT_LIMIT_COUNT,
    };

    TEST("elosEventBufferNew");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventBufferNew(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, param");
    result = elosEventBufferNew(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&eventBufferPtr, NULL");
    result = elosEventBufferNew(&eventBufferPtr, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
