// SPDX-License-Identifier: MIT

#include "elosEventSourceDeserialize_utest.h"

int elosTestElosEventSourceDeserializeSuccessEmptySetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeserializeSuccessEmptyTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeserializeSuccessEmpty(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    const char string[] = "{}";
    elosEventSource_t receivedSource;

    TEST("elosEventSourceDeserialize");
    SHOULD("%s", "succeed for an empty jsonString");

    result = elosEventSourceDeserialize(&receivedSource, string);
    assert_int_equal(result, SAFU_RESULT_OK);
}
