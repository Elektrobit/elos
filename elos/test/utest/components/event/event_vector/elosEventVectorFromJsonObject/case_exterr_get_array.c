// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonObject_utest.h"
#include "safu/mock_safu.h"

int elosTestElosEventVectorFromJsonObjectExtErrGetArraySetup(void **state) {
    elosEventVectorFromJsonObjectUtestCreateEventVectorJobj(state);
    return 0;
}

int elosTestElosEventVectorFromJsonObjectExtErrGetArrayTeardown(void **state) {
    elosEventVectorFromJsonObjectUtestDeleteEventVectorJobj(state);
    return 0;
}

void elosTestElosEventVectorFromJsonObjectExtErrGetArray(void **state) {
    elosUteststateT_t *testState = *state;
    elosEventVector_t *eventVector = NULL;
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventVectorFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when safuJsonGetArray failed");

    MOCK_FUNC_AFTER_CALL(safuJsonGetArray, 0);
    expect_value(__wrap_safuJsonGetArray, jobj, testState->eventVecJobj);
    expect_string(__wrap_safuJsonGetArray, name, "eventArray");
    expect_value(__wrap_safuJsonGetArray, idx, 0);
    expect_any(__wrap_safuJsonGetArray, len);
    will_set_parameter(__wrap_safuJsonGetArray, len, NUM_TEST_EVENTS);
    will_return(__wrap_safuJsonGetArray, NULL);

    result = elosEventVectorFromJsonObject(testState->eventVecJobj, &eventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
