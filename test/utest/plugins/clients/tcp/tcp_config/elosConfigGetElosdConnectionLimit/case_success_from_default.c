// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosConfigGetElosdConnectionLimit_utest.h"

int elosTestElosConfigGetElosdConnectionLimitSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdConnectionLimitSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdConnectionLimitSuccessFromDefault(UNUSED void **state) {
    TEST("elosTcpConfigGetConnectionLimit");
    SHOULD("%s", "get the elos connection limit default option");

    samconfConfig_t mockConfig = {0};
    samconfUtilCreateMockConfigFromStr("{\"Config\":{}}", false, &mockConfig);
    elosPlugin_t plugin = {.useEnv = false, .config = &mockConfig};

    int32_t expectedValue = 200;
    int32_t limit = elosTcpConfigGetConnectionLimit(&plugin);
    assert_int_equal(expectedValue, limit);

    samconfConfigDeleteMembers(&mockConfig);
}
