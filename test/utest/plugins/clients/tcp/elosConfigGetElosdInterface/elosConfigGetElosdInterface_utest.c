// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdInterface_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdInterfaceUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdInterfaceSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdInterfaceSuccessFromEnv),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdInterfaceUtest);
}

static int elosTestElosConfigGetElosdInterfaceUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdInterfaceUtestTeardown(UNUSED void **state) {
    return 0;
}
