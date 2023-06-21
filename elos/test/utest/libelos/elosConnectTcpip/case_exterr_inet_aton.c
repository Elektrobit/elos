// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosConnectTcpip_utest.h"

int elosTestElosConnectTcpipExtErrInetAtonSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConnectTcpipExtErrInetAtonTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConnectTcpipExtErrInetAton(UNUSED void **state) {
    elosSession_t *newSession = NULL;
    safuResultE_t result;

    TEST("elosConnectTcpip");
    SHOULD("%s", "return SAFU_RESULT_FAILED when calling inet_aton fails");

    MOCK_FUNC_AFTER_CALL(inet_aton, 0);
    expect_value(__wrap_inet_aton, cp, MOCK_IP_ADDR);
    expect_any(__wrap_inet_aton, inp);
    will_return(__wrap_inet_aton, -1);

    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
