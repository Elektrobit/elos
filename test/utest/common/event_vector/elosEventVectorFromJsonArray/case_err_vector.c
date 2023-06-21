// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonArray_utest.h"
#include "mock_event.h"
#include "mock_event_vector.h"
#include "safu/mock_vector.h"

int elosTestElosEventVectorFromJsonArrayErrVectorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorFromJsonArrayErrVectorTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventVectorDelete(test->eventVector);
    test->eventVector = NULL;

    return 0;
}

void elosTestElosEventVectorFromJsonArrayErrVector(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventVectorFromJsonArray");
    SHOULD("%s", "return SAFU_RESULT_FAILED after errors during eventVector creation");

    PARAM("%s", "elosEventVectorNew fails");

    MOCK_FUNC_AFTER_CALL(elosEventVectorNew, 0);
    expect_not_value(elosEventVectorNew, eventVector, 0);
    expect_not_value(elosEventVectorNew, elements, 0);
    will_set_parameter(elosEventVectorNew, eventVector, NULL);
    will_return(elosEventVectorNew, SAFU_RESULT_FAILED);

    result = elosEventVectorFromJsonArray(test->jEventArray, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "json_object_array_get_idx fails");

    MOCK_FUNC_AFTER_CALL(json_object_array_get_idx, 1);
    expect_not_value(__wrap_json_object_array_get_idx, obj, NULL);
    expect_value(__wrap_json_object_array_get_idx, idx, 1);
    will_return(__wrap_json_object_array_get_idx, NULL);

    result = elosEventVectorFromJsonArray(test->jEventArray, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosEventNew fails");

    MOCK_FUNC_AFTER_CALL(elosEventNew, 1);
    expect_not_value(elosEventNew, event, NULL);
    will_return(elosEventNew, SAFU_RESULT_FAILED);

    result = elosEventVectorFromJsonArray(test->jEventArray, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "elosEventFromJsonObject fails");

    MOCK_FUNC_AFTER_CALL(elosEventFromJsonObject, 1);
    expect_not_value(elosEventFromJsonObject, to, NULL);
    expect_not_value(elosEventFromJsonObject, from, NULL);
    will_return(elosEventFromJsonObject, SAFU_RESULT_FAILED);

    result = elosEventVectorFromJsonArray(test->jEventArray, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "safuVecPush fails");

    MOCK_FUNC_AFTER_CALL(safuVecPush, 1);
    expect_not_value(__wrap_safuVecPush, vec, NULL);
    expect_not_value(__wrap_safuVecPush, element, NULL);
    will_return(__wrap_safuVecPush, -1);

    result = elosEventVectorFromJsonArray(test->jEventArray, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
