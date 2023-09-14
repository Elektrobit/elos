// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonArray_utest.h"
#include "mock_event.h"
#include "safu/mock_vector.h"

int elosTestElosEventVectorToJsonArrayErrArrayAddSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorToJsonArrayErrArrayAddTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorToJsonArrayErrArrayAdd(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventVectorToJsonArray");
    SHOULD("%s", "return SAFU_RESULT_FAILED when adding an array element failed");

    PARAM("%s", "safuVecGet fails");

    MOCK_FUNC_AFTER_CALL(safuVecGet, 1);
    expect_not_value(__wrap_safuVecGet, vec, NULL);
    expect_value(__wrap_safuVecGet, idx, 1);
    will_return(__wrap_safuVecGet, NULL);

    result = elosEventVectorToJsonArray(test->eventVector, &test->jEventArray);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "json_object_new_object fails");

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 1);
    will_return(__wrap_json_object_new_object, NULL);

    result = elosEventVectorToJsonArray(test->eventVector, &test->jEventArray);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosEventToJsonObject fails");

    MOCK_FUNC_AFTER_CALL(elosEventToJsonObject, 1);
    expect_not_value(elosEventToJsonObject, to, NULL);
    expect_not_value(elosEventToJsonObject, from, NULL);
    will_return(elosEventToJsonObject, SAFU_RESULT_FAILED);

    result = elosEventVectorToJsonArray(test->eventVector, &test->jEventArray);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "json_object_array_put_idx fails");

    MOCK_FUNC_AFTER_CALL(json_object_array_put_idx, 1);
    expect_not_value(__wrap_json_object_array_put_idx, obj, NULL);
    expect_value(__wrap_json_object_array_put_idx, idx, 1);
    expect_not_value(__wrap_json_object_array_put_idx, val, NULL);
    will_return(__wrap_json_object_array_put_idx, -1);

    result = elosEventVectorToJsonArray(test->eventVector, &test->jEventArray);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
