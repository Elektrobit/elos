// SPDX-License-Identifier: MIT
#include "elosClientAuthorizationInitialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientAuthorizationInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosClientAuthorizationInitializeErrParam),
        TEST_CASE(elosTestElosClientAuthorizationInitializeExterrMnlSocketBind),
        TEST_CASE(elosTestElosClientAuthorizationInitializeExterrMnlSocketOpen),
        TEST_CASE(elosTestElosClientAuthorizationInitializeSuccess),
    };
    return RUN_TEST_SUITE(tests, elosClientAuthorizationInitializeUtest);
}

static int elosClientAuthorizationInitializeUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosClientAuthorizationInitializeUtestTeardown(UNUSED void **state) {
    return 0;
}
