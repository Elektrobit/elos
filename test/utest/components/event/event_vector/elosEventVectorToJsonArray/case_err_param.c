// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonArray_utest.h"

int elosTestElosEventVectorToJsonArrayErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorToJsonArrayErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorToJsonArrayErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventVectorToJsonArray");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid param");

    PARAM("%s", "&eventVector, NULL");
    result = elosEventVectorToJsonArray(test->eventVector, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
