// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonArray_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventVectorToJsonArraySuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorToJsonArraySuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorToJsonArraySuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *sEventVector;
    safuResultE_t result;

    TEST("elosEventVectorToJsonArray");
    SHOULD("%s", "successfully transfer an event vector to a json object");

    PARAM("%s", "elosEventVectorToJsonArray with Events");

    result = elosEventVectorToJsonArray(test->eventVector, &test->jEventArray);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->jEventArray);

    sEventVector = json_object_to_json_string_ext(test->jEventArray, JSON_C_TO_STRING_PLAIN);
    assert_string_equal(sEventVector, "[{\"source\":{},\"messageCode\":7000},{\"source\":{},\"messageCode\":8000}]");

    json_object_put(test->jEventArray);
    test->jEventArray = NULL;

    PARAM("%s", "elosEventVectorToJsonArray without Events");

    result = elosEventVectorToJsonArray(NULL, &test->jEventArray);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->jEventArray);

    sEventVector = json_object_to_json_string_ext(test->jEventArray, JSON_C_TO_STRING_PLAIN);
    assert_string_equal(sEventVector, "[]");

    json_object_put(test->jEventArray);
    test->jEventArray = NULL;
}
