// SPDX-License-Identifier: MIT

#include "elosEventVectorSerialize_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventVectorSerializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorSerializeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorSerializeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char *sEventVector;
    safuResultE_t result;

    TEST("elosEventVectorSerialize");
    SHOULD("%s", "successfully convert a EventVector into a JsonString");

    PARAM("%s", "elosEventVectorSerialize with Events");

    result = elosEventVectorSerialize(test->eventVector, &sEventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(sEventVector);
    assert_string_equal(sEventVector, "[{\"source\":{},\"messageCode\":7000},{\"source\":{},\"messageCode\":8000}]");

    free(sEventVector);
    sEventVector = NULL;

    PARAM("%s", "elosEventVectorSerialize without Events");

    result = elosEventVectorSerialize(NULL, &sEventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(sEventVector);
    assert_string_equal(sEventVector, "[]");

    free(sEventVector);
}
