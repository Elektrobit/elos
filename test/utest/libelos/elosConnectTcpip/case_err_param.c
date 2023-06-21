// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosConnectTcpip_utest.h"

int elosTestElosConnectTcpipErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConnectTcpipErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConnectTcpipErrParam(UNUSED void **state) {
    elosSession_t *newSession = NULL;
    safuResultE_t result;

    TEST("elosConnectTcpip");
    SHOULD("%s", "return SAFU_RESULT_FAILED when calling elosConnectTcpip with wrong parameters");

    PARAM("%s", "NULL, 0, NULL");
    result = elosConnectTcpip(NULL, 0, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, port, &session");
    result = elosConnectTcpip(NULL, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "ip, port, NULL");
    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
