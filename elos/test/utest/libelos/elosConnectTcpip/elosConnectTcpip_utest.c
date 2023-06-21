// SPDX-License-Identifier: MIT
#include "elosConnectTcpip_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosConnectTcpipUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConnectTcpipErrParam),       TEST_CASE(elosTestElosConnectTcpipExtErrAllocMem),
        TEST_CASE(elosTestElosConnectTcpipExtErrInetAton), TEST_CASE(elosTestElosConnectTcpipExtErrSocket),
        TEST_CASE(elosTestElosConnectTcpipExtErrConnect),  TEST_CASE(elosTestElosConnectTcpipSuccess),
    };

    return RUN_TEST_SUITE(tests, elosConnectTcpipUtest);
}

elosSession_t *elosConnectTcpipCreateSession() {
    elosSession_t *session = safuAllocMem(NULL, sizeof(elosSession_t));
    assert_non_null(session);
    session->addr.sin_family = AF_INET;
    session->addr.sin_port = htons(MOCK_PORT);
    session->addr.sin_addr.s_addr = inet_addr(MOCK_IP_ADDR);
    session->fd = MOCK_FD;
    return session;
}

static int elosConnectTcpipUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosConnectTcpipUtestTeardown(UNUSED void **state) {
    return 0;
}
