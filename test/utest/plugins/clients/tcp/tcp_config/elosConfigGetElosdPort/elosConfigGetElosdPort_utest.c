// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdPort_utest.h"

#include <samconf/samconf_types.h>

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdPortUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdPortSuccessFromConfig),
        TEST_CASE(elosTestElosConfigGetElosdPortSuccessFromDefault),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdPortUtest);
}

samconfConfig_t elosGetMockConfig() {
    samconfConfig_t configData = {
        .parent = NULL,
        .key = "tcp",
        .type = SAMCONF_CONFIG_VALUE_OBJECT,
        .children = NULL,
        .childCount = 0,
    };
    return configData;
}

static int elosTestElosConfigGetElosdPortUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdPortUtestTeardown(UNUSED void **state) {
    return 0;
}
