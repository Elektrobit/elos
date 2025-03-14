// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosUnixConfigGetPath_utest.h"

int elosTestelosUnixConfigGetPathSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixConfigGetPathSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixConfigGetPathSuccessFromDefault(UNUSED void **state) {
    TEST("elosUnixConfigGetPath");
    SHOULD("%s", "get the elos path default option");

    samconfConfig_t mockConfig = {0};
    samconfUtilCreateMockConfigFromStr("{\"Config\":{}}", false, &mockConfig);
    elosPlugin_t plugin = {.useEnv = false, .config = &mockConfig};

    const char *path = elosUnixConfigGetPath(&plugin);
    samconfConfigDeleteMembers(&mockConfig);
    assert_string_equal(ELOSD_SOCKET_PATH, path);
}
