// SPDX-License-Identifier: MIT

#include "elosEventVectorSerialize_utest.h"

int elosTestElosEventVectorSerializeErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorSerializeErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorSerializeErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventVectorSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    // Note: eventVector is allowed to be NULL (tested in case_success.c)
    PARAM("%s", "&eventVector, NULL");
    result = elosEventVectorSerialize(test->eventVector, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
