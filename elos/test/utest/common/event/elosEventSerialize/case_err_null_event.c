// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeErrorNullEventSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSerializeErrorNullEventTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSerializeErrorNullEvent(UNUSED void **state) {
    char *result = NULL;
    safuResultE_t retval = SAFU_RESULT_OK;

    TEST("elosEventSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the serialized event is null");

    retval = elosEventSerialize(&result, NULL);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
