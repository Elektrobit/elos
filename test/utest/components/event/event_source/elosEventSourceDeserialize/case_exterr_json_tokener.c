// SPDX-License-Identifier: MIT

#include "elosEventSourceDeserialize_utest.h"

int elosTestElosEventSourceDeserializeExterrJsonTokenerSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeserializeExterrJsonTokenerTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeserializeExterrJsonTokener(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEventSource_t *receivedSource = NULL;

    TEST("elosEventSourceDeserialize");
    SHOULD("%s", "return NULL if json_tokener_parse fails");

    result = elosEventSourceDeserialize(receivedSource, "INVALIDJSONSTRING");
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
