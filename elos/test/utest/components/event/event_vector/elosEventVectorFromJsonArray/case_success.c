// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonArray_utest.h"

int elosTestElosEventVectorFromJsonArraySuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorFromJsonArraySuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventVectorDelete(test->eventVector);
    test->eventVector = NULL;

    return 0;
}

void elosTestElosEventVectorFromJsonArraySuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEvent_t *element;
    safuResultE_t result;

    TEST("elosEventVectorFromJsonArray");
    SHOULD("%s", "successfully transfer an event vector to a json object");

    PARAM("%s", "elosEventVectorFromJsonArray with Events");

    result = elosEventVectorFromJsonArray(test->jEventArray, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->eventVector);
    assert_int_equal(test->eventVector->elementCount, 2);
    element = safuVecGetLast(test->eventVector);
    assert_non_null(element);
    assert_int_equal(element->messageCode, 8000);

    elosEventVectorDelete(test->eventVector);

    PARAM("%s", "elosEventVectorFromJsonArray without Events");

    result = elosEventVectorFromJsonArray(test->jEventArrayEmpty, &test->eventVector);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_null(test->eventVector);
}
