// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosConfigGetElosdInterface_utest.h"

int elosTestElosConfigGetElosdInterfaceSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdInterfaceSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdInterfaceSuccessFromDefault(UNUSED void **state) {
    TEST("elosTcpConfigGetInterface");
    SHOULD("%s", "get the elos interface default option");

    samconfConfig_t mockConfig = {0};
    samconfUtilCreateMockConfigFromStr("{\"Config\":{}}", false, &mockConfig);
    elosPlugin_t plugin = {.useEnv = false, .config = &mockConfig};

    const char *returnValue = elosTcpConfigGetInterface(&plugin);
    assert_string_equal(returnValue, ELOSD_INTERFACE);

    samconfConfigDeleteMembers(&mockConfig);
}
