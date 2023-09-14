// SPDX-License-Identifier: MIT
#include "elosClientAuthorizationIsTrustedConnection_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientAuthorizationIsTrustedConnectionUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosClientAuthorizationIsTrustedConnectionSuccess),
    };
    return RUN_TEST_SUITE(tests, elosClientAuthorizationIsTrustedConnectionUtest);
}

static int elosClientAuthorizationIsTrustedConnectionUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosClientAuthorizationIsTrustedConnectionUtestTeardown(UNUSED void **state) {
    return 0;
}
