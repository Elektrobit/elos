// SPDX-License-Identifier: MIT
#include "elosBlacklistDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosBlacklistDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosBlacklistDeleteSuccess),
        TEST_CASE(elosTestElosBlacklistDeleteErrBlacklistParameterNull),
        TEST_CASE(elosTestElosBlacklistDeleteErrBlacklistFilterDelete),
    };
    return RUN_TEST_SUITE(tests, elosBlacklistDeleteUtest);
}

static int elosBlacklistDeleteUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosBlacklistDeleteUtestTeardown(UNUSED void **state) {
    return 0;
}
