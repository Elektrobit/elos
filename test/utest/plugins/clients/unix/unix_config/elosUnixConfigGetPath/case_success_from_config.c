// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosUnixConfigGetPath_utest.h"

#define MOCK_SOCKET_PATH "/run/elosd/elosd.socket"

int elosTestelosUnixConfigGetPathSuccessFromConfigSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"path\": \"" MOCK_SOCKET_PATH
        "\""
        "  }"
        "}",
        true, config);
    *state = config;
    return 0;
}

int elosTestelosUnixConfigGetPathSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestelosUnixConfigGetPathSuccessFromConfig(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    TEST("elosUnixConfigGetPath");
    SHOULD("%s", "get the elos path environment option");

    elosPlugin_t plugin = {.useEnv = true, .config = config};

    const char *path = elosUnixConfigGetPath(&plugin);
    assert_string_equal(path, MOCK_SOCKET_PATH);
}
