// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdConnectionLimit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdConnectionLimitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdConnectionLimitSuccessFromConfig),
        TEST_CASE(elosTestElosConfigGetElosdConnectionLimitSuccessFromDefault),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdConnectionLimitUtest);
}

static int elosTestElosConfigGetElosdConnectionLimitUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdConnectionLimitUtestTeardown(UNUSED void **state) {
    return 0;
}
