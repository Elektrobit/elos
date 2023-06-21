// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonObject_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventVectorToJsonObjectExtErrAddNewArraySetup(void **state) {
    elosEventVectorToJsonObjectUtestCreateEventVector(state);
    return 0;
}

int elosTestElosEventVectorToJsonObjectExtErrAddNewArrayTeardown(void **state) {
    elosEventVectorToJsonObjectUtestDeleteEventVector(state);
    return 0;
}

void elosTestElosEventVectorToJsonObjectExtErrAddNewArray(void **state) {
    elosUteststateT_t *testState = *state;
    json_object *eventVecJobj = NULL;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorToJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when safuJsonAddNewArrayOfInitialSize failed");

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewArrayOfInitialSize, 0);
    expect_not_value(__wrap_safuJsonAddNewArrayOfInitialSize, jobj, NULL);
    expect_string(__wrap_safuJsonAddNewArrayOfInitialSize, name, "eventArray");
    expect_value(__wrap_safuJsonAddNewArrayOfInitialSize, initialSize, testState->eventVector->elementCount);
    will_return(__wrap_safuJsonAddNewArrayOfInitialSize, NULL);

    result = elosEventVectorToJsonObject(testState->eventVector, &eventVecJobj);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
