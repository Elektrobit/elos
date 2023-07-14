// SPDX-License-Identifier: MIT

#include "elosEventDeserialize_utest.h"

int elosTestElosEventDeserializeEmptyInputStringSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeserializeEmptyInputStringTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeserializeEmptyInputString(UNUSED void **state) {
    safuResultE_t retval;
    elosEvent_t resultEvent = {0};

    TEST("elosEventDeserialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if empty string is deserialized");

    retval = elosEventDeserialize(&resultEvent, NULL);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
