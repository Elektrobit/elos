// SPDX-License-Identifier: MIT

#include "elosEventDeserialize_utest.h"

int elosTestElosEventDeserializeEmptyEventSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeserializeEmptyEventTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeserializeEmptyEvent(UNUSED void **state) {
    safuResultE_t retval;
    const char *testData = "{\"payload\":\"payload\"}";

    TEST("elosEventDeserialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the event parameter is NULL");

    retval = elosEventDeserialize(NULL, testData);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
