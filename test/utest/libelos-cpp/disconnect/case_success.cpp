// SPDX-License-Identifier: MIT
#include "disconnect_utest.h"

int elosTestDisconnectSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestDisconnectSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestDisconnectSuccess(UNUSED void **state) {
    using namespace elos;
    elosSession_t *testSession = NULL;
    safuResultE_t result;
    Elos testObject;

    TEST("disconnect");
    SHOULD("%s", "successfully close a connection");

    result = testObject.disconnect(&testSession);
    assert_int_equal(result, SAFU_RESULT_OK);
}
