// SPDX-License-Identifier: MIT

#include "elosEventSourceSerialize_utest.h"

int elosTestElosEventSourceSerializeErrNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceSerializeErrNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceSerializeErrNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    char *receivedString = NULL;

    TEST("elosEventSourceSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if the event parameter is NULL");

    result = elosEventSourceSerialize(&receivedString, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
