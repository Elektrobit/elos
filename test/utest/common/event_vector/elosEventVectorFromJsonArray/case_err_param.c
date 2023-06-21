// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonArray_utest.h"

int elosTestElosEventVectorFromJsonArrayErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorFromJsonArrayErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorFromJsonArrayErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventVectorFromJsonArray");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid param");

    PARAM("%s", "NULL, NULL");
    result = elosEventVectorFromJsonArray(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&jEventArray, NULL");
    result = elosEventVectorFromJsonArray(test->jEventArray, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &eventVector");
    result = elosEventVectorFromJsonArray(NULL, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
