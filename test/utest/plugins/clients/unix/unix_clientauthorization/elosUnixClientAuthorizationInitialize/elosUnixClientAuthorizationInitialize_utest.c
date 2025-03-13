// SPDX-License-Identifier: MIT
#include "elosUnixClientAuthorizationInitialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosUnixClientAuthorizationInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestelosUnixClientAuthorizationInitializeErrParam),
        TEST_CASE(elosTestelosUnixClientAuthorizationInitializeSuccess),
    };
    return RUN_TEST_SUITE(tests, elosUnixClientAuthorizationInitializeUtest);
}

static int elosUnixClientAuthorizationInitializeUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosUnixClientAuthorizationInitializeUtestTeardown(UNUSED void **state) {
    return 0;
}
