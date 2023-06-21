// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonObject_utest.h"

int elosTestElosEventVectorToJsonObjectExtErrNewJobjSetup(void **state) {
    elosEventVectorToJsonObjectUtestCreateEventVector(state);
    return 0;
}

int elosTestElosEventVectorToJsonObjectExtErrNewJobjTeardown(void **state) {
    elosEventVectorToJsonObjectUtestDeleteEventVector(state);
    return 0;
}

void elosTestElosEventVectorToJsonObjectExtErrNewJobj(void **state) {
    elosUteststateT_t *testState = *state;
    json_object *eventVecJobj = NULL;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorToJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when json_object_new_object failed");

    will_return(__wrap_json_object_new_object, NULL);
    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    result = elosEventVectorToJsonObject(testState->eventVector, &eventVecJobj);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
