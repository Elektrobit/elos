// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonArray_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventVectorToJsonArrayErrArrayNewSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorToJsonArrayErrArrayNewTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorToJsonArrayErrArrayNew(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    json_object *jEventArray = NULL;
    safuResultE_t result;

    TEST("elosEventVectorToJsonArray");
    SHOULD("%s", "return SAFU_RESULT_FAILED when json_object_new_array_ext failed");

    MOCK_FUNC_AFTER_CALL(json_object_new_array_ext, 0);
    expect_any(__wrap_json_object_new_array_ext, initial_size);
    will_return(__wrap_json_object_new_array_ext, NULL);

    result = elosEventVectorToJsonArray(test->eventVector, &jEventArray);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
