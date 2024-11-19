// SPDX-License-Identifier: MIT
#include "elosConnectSessionTcpip_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosConnectSessionTcpipUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConnectSessionTcpipExtErrGetaddrinfo),
        TEST_CASE(elosTestElosConnectSessionTcpipExtErrSocket),
        TEST_CASE(elosTestElosConnectSessionTcpipExtErrConnect),
        TEST_CASE(elosTestElosConnectSessionTcpipSuccess),
    };

    return RUN_TEST_SUITE(tests, elosConnectSessionTcpipUtest);
}

static int elosConnectSessionTcpipUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosConnectSessionTcpipUtestTeardown(UNUSED void **state) {
    return 0;
}
