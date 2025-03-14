// SPDX-License-Identifier: MIT
#include "elosUnixClientAuthorizationDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosUnixClientAuthorizationDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestelosUnixClientAuthorizationDeleteErrParam),
        TEST_CASE(elosTestelosUnixClientAuthorizationDeleteSuccess),
    };
    return RUN_TEST_SUITE(tests, elosUnixClientAuthorizationDeleteUtest);
}

static int elosUnixClientAuthorizationDeleteUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosUnixClientAuthorizationDeleteUtestTeardown(UNUSED void **state) {
    return 0;
}
