// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosConnectTcpip_utest.h"

int elosTestElosConnectTcpipExtErrGetaddrinfoSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConnectTcpipExtErrGetaddrinfoTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConnectTcpipExtErrGetaddrinfo(UNUSED void **state) {
    elosSession_t *newSession = NULL;
    safuResultE_t result;

    TEST("elosConnectTcpip");
    SHOULD("%s", "return SAFU_RESULT_FAILED when calling getaddrinfo fails");

    MOCK_FUNC_AFTER_CALL(getaddrinfo, 0);
    expect_value(__wrap_getaddrinfo, node, MOCK_IP_ADDR);
    expect_any(__wrap_getaddrinfo, service);
    expect_any(__wrap_getaddrinfo, hints);
    will_set_parameter(__wrap_getaddrinfo, res, NULL);
    will_return(__wrap_getaddrinfo, -1);

    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
