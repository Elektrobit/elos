// SPDX-License-Identifier: MIT
#include "elosUnixClientAuthorizationIsTrustedConnection_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosUnixClientAuthorizationIsTrustedConnectionUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestelosUnixClientAuthorizationIsTrustedConnectionSuccess),
    };
    return RUN_TEST_SUITE(tests, elosUnixClientAuthorizationIsTrustedConnectionUtest);
}

static int elosUnixClientAuthorizationIsTrustedConnectionUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosUnixClientAuthorizationIsTrustedConnectionUtestTeardown(UNUSED void **state) {
    return 0;
}
