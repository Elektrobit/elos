// SPDX-License-Identifier: MIT
#include "elosClientAuthorizationDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientAuthorizationDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosClientAuthorizationDeleteErrParam),
        TEST_CASE(elosTestElosClientAuthorizationDeleteExterrMnlSocketClose),
        TEST_CASE(elosTestElosClientAuthorizationDeleteSuccess),
        TEST_CASE(elosTestElosClientAuthorizationDeleteSuccessSocketIsNull),
    };
    return RUN_TEST_SUITE(tests, elosClientAuthorizationDeleteUtest);
}

static int elosClientAuthorizationDeleteUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosClientAuthorizationDeleteUtestTeardown(UNUSED void **state) {
    return 0;
}
