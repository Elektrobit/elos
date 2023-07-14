// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonObject_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventVectorToJsonObjectSuccessSetup(void **state) {
    elosEventVectorToJsonObjectUtestCreateEventVector(state);
    return 0;
}

int elosTestElosEventVectorToJsonObjectSuccessTeardown(void **state) {
    elosEventVectorToJsonObjectUtestDeleteEventVector(state);
    return 0;
}

void elosTestElosEventVectorToJsonObjectSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    json_object *eventVecJobj = NULL;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosEventVectorToJsonObject");
    SHOULD("%s", "successfully transfer an event vector to a json object");

    result = elosEventVectorToJsonObject(testState->eventVector, &eventVecJobj);
    assert_int_equal(result, SAFU_RESULT_OK);

    json_object_put(eventVecJobj);
}
