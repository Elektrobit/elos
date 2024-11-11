// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosDisconnectSession_utest.h"

int elosTestElosDisconnectSessionSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosDisconnectSessionSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosDisconnectSessionSuccess(UNUSED void **state) {
    safuResultE_t result;
    elosSession_t session = {MOCK_FD, true};

    TEST("elosDisconnect");
    SHOULD("%s", "successfully close a connection");

    PARAM("%s", "With session");
    expect_value(__wrap_close, fd, MOCK_FD);
    will_return(__wrap_close, 0);
    MOCK_FUNC_AFTER_CALL(close, 0);

    result = elosDisconnectSession(&session);
    assert_int_equal(session.fd, 0);
    assert_int_equal(session.connected, false);
    assert_int_equal(result, SAFU_RESULT_OK);
}
