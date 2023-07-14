// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeErrorNullStorageStringSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSerializeErrorNullStorageStringTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSerializeErrorNullStorageString(UNUSED void **state) {
    safuResultE_t retval = SAFU_RESULT_OK;
    elosEvent_t testEvent = {0};

    TEST("elosEventSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the result storage string is null");

    retval = elosEventSerialize(NULL, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
