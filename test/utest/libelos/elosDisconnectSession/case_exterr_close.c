// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosDisconnectSession_utest.h"

int elosTestElosDisconnectSessionExtErrCloseSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosDisconnectSessionExtErrCloseTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosDisconnectSessionExtErrClose(UNUSED void **state) {
    safuResultE_t result;
    elosSession_t session = {MOCK_FD, false};

    TEST("elosDisconnect");
    SHOULD("%s", "return -1 if close fails");

    expect_value(__wrap_close, fd, MOCK_FD);
    will_return(__wrap_close, -1);
    MOCK_FUNC_AFTER_CALL(close, 0);

    result = elosDisconnectSession(&session);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
