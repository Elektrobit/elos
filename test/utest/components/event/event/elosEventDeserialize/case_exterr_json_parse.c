// SPDX-License-Identifier: MIT

#include "elosEventDeserialize_utest.h"

int elosTestElosEventDeserializeJsonParseErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeserializeJsonParseErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeserializeJsonParseError(UNUSED void **state) {
    safuResultE_t retval;
    elosEvent_t resultEvent = {0};
    const char *testData = "invalidBrokenJsonString {!}";

    TEST("elosEventDeserialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED as new json object creation fails");

    retval = elosEventDeserialize(&resultEvent, testData);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
