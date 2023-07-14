// SPDX-License-Identifier: MIT

#include "elosEventDeserialize_utest.h"
#include "mock_event.h"

int elosTestElosEventDeserializeEventConversionErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeserializeEventConversionErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeserializeEventConversionError(UNUSED void **state) {
    safuResultE_t retval;
    elosEvent_t resultEvent = {0};
    const char *testData =
        "{\"timestamp\":[25,100],\"severity\":1,\"hardwareid\":\"localhost\","
        "\"classification\":42,\"messageCode\":32,\"payload\":\"payload\"}";

    TEST("elosEventDeserialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED as event to json conversion fails");

    MOCK_FUNC_AFTER_CALL(elosEventFromJsonObject, 0);
    expect_not_value(elosEventFromJsonObject, to, NULL);
    expect_not_value(elosEventFromJsonObject, from, NULL);
    will_return(elosEventFromJsonObject, SAFU_RESULT_FAILED);

    retval = elosEventDeserialize(&resultEvent, testData);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
