// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorProcess_utest.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorProcessErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterNodeVectorProcessErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterNodeVectorProcessErrParam(UNUSED void **state) {
    safuResultE_t result;
    elosEventFilterNodeVector_t eventFilterNodeVector = {0};
    elosEvent_t const event = {0};

    TEST("elosEventFilterNodeVectorProcess");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    // Note: We do not need to test the second parameter here, as NULL is also a valid value in this case

    PARAM("%s", "NULL, NULL, NULL");
    result = elosEventFilterNodeVectorProcess(NULL, NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "&eventFilterNode, NULL, NULL");
    result = elosEventFilterNodeVectorProcess(&eventFilterNodeVector, NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, NULL, &event");
    result = elosEventFilterNodeVectorProcess(NULL, NULL, &event);
    assert_true(result == SAFU_RESULT_FAILED);
}
