// SPDX-License-Identifier: MIT
#include "elosMessageHandlerHandleMessage_utest.h"

int elosTestElosMessageHandlerHandleMessageErrConnectionSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosMessageHandlerHandleMessageErrConnectionTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosMessageHandlerHandleMessageErrConnection(UNUSED void **state) {
    safuResultE_t retval;

    TEST("elosMessageHandlerHandleMessage");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the connection parameter is null");

    retval = elosMessageHandlerHandleMessage(NULL);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
