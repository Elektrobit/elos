// SPDX-License-Identifier: MIT

#include "elosEventSourceDeserialize_utest.h"

int elosTestElosEventSourceDeserializeErrNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeserializeErrNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeserializeErrNull(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEventSource_t *receivedSource = NULL;

    TEST("elosEventSourceDeserialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if the jsonString parameter is NULL");

    result = elosEventSourceDeserialize(receivedSource, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
