// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elosEventVectorSerialize_utest.h"
#include "mock_event_vector.h"

int elosTestElosEventVectorSerializeErrConversionSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorSerializeErrConversionTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorSerializeErrConversion(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char *sEventVector = NULL;
    safuResultE_t result;

    TEST("elosEventVectorSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED when adding an array element failed");

    PARAM("%s", "elosEventVectorToJsonArray fails");

    MOCK_FUNC_AFTER_CALL(elosEventVectorToJsonArray, 0);
    expect_any(elosEventVectorToJsonArray, eventVector);
    expect_not_value(elosEventVectorToJsonArray, jEventVector, NULL);
    will_set_parameter(elosEventVectorToJsonArray, jEventVector, NULL);
    will_return(elosEventVectorToJsonArray, SAFU_RESULT_FAILED);

    result = elosEventVectorSerialize(test->eventVector, &sEventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(sEventVector);

    PARAM("%s", "json_object_to_json_string_ext fails");

    MOCK_FUNC_AFTER_CALL(json_object_to_json_string_ext, 0);
    expect_not_value(__wrap_json_object_to_json_string_ext, obj, NULL);
    expect_value(__wrap_json_object_to_json_string_ext, flags, JSON_C_TO_STRING_PLAIN);
    will_return(__wrap_json_object_to_json_string_ext, NULL);

    result = elosEventVectorSerialize(test->eventVector, &sEventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(sEventVector);

    PARAM("%s", "strdup fails");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_not_value(__wrap_strdup, string, NULL);
    will_return(__wrap_strdup, NULL);

    result = elosEventVectorSerialize(test->eventVector, &sEventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(sEventVector);
}
