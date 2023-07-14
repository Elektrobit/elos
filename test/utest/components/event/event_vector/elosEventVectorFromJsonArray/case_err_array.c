// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonArray_utest.h"
#include "mock_event_vector.h"

int elosTestElosEventVectorFromJsonArrayErrArraySetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorFromJsonArrayErrArrayTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventVectorDelete(test->eventVector);
    test->eventVector = NULL;

    return 0;
}

void elosTestElosEventVectorFromJsonArrayErrArray(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventVectorFromJsonArray");
    SHOULD("%s", "return SAFU_RESULT_FAILED after errors reading the json event array");

    PARAM("%s", "jEventArray is not of type 'json_type_array'");

    result = elosEventVectorFromJsonArray(test->jEventArrayInvalid, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
