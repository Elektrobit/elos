// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosUnixConfigGetPath_utest.h"

static char *elosCurrentEnvValue = NULL;

int elosTestelosUnixConfigGetPathSuccessFromEnvSetup(UNUSED void **state) {
    char *envValue = getenv("ELOSD_SOCKET_PATH");
    if (envValue != NULL) {
        elosCurrentEnvValue = strdup(envValue);
    }
    return 0;
}

int elosTestelosUnixConfigGetPathSuccessFromEnvTeardown(UNUSED void **state) {
    if (elosCurrentEnvValue != NULL) {
        setenv("ELOSD_SOCKET_PATH", elosCurrentEnvValue, 1);
        free(elosCurrentEnvValue);
    }
    return 0;
}

void elosTestelosUnixConfigGetPathSuccessFromEnv(UNUSED void **state) {
    TEST("elosUnixConfigGetPath");
    SHOULD("%s", "get the elos path environment option");

    samconfConfig_t mockConfig = {0};
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\": {"
        "    \"path\": /run/elosd/elosd.socket"
        "  }"
        "}",
        true, &mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
    setenv("ELOSD_SOCKET_PATH", "/tmp/elosd.socket", 1);  // environment is restored by setup/teardown
    elosPlugin_t plugin = {.useEnv = true, .config = &mockConfig};

    const char *path = elosUnixConfigGetPath(&plugin);
    samconfConfigDeleteMembers(&mockConfig);
    assert_string_equal("/tmp/elosd.socket", path);
}
