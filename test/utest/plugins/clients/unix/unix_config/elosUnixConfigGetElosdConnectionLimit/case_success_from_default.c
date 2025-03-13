// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosUnixConfigGetElosdConnectionLimit_utest.h"

int elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixConfigGetElosdConnectionLimitSuccessFromDefault(UNUSED void **state) {
    TEST("elosUnixConfigGetConnectionLimit");
    SHOULD("%s", "get the elos connection limit default option");

    samconfConfig_t mockConfig = {0};
    samconfUtilCreateMockConfigFromStr("{\"Config\":{}}", false, &mockConfig);
    elosPlugin_t plugin = {.useEnv = false, .config = &mockConfig};

    int32_t expectedValue = 200;
    int32_t limit = elosUnixConfigGetConnectionLimit(&plugin);
    samconfConfigDeleteMembers(&mockConfig);
    assert_int_equal(expectedValue, limit);
}
