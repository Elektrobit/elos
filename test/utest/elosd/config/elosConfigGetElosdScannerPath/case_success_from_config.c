// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosConfigGetElosdScannerPath_utest.h"

#define MOCK_SCANNER_PATH "build/lib/elos/scanner"
#define MOCK_CONFIG                                             \
    "{                                                          \
        \"root\": {                                             \
            \"elos\": {                                         \
                \"UseEnv\": false,                              \
                \"Scanner\": {                                  \
                    \"PluginSearchPath\": \"" MOCK_SCANNER_PATH \
    "\" \
                }                                               \
            }                                                   \
        }                                                       \
    }"

int elosTestElosConfigGetElosdScannerPathSuccessFromConfigSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(MOCK_CONFIG, true, config);
    *state = config;
    return 0;
}

int elosTestElosConfigGetElosdScannerPathSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestElosConfigGetElosdScannerPathSuccessFromConfig(void **state) {
    samconfConfig_t *mockConfig = *(samconfConfig_t **)state;

    TEST("elosConfigGetElosdScannerPath");
    SHOULD("%s", "get the elos scanner path environment option");

    const char *returnValue = elosConfigGetElosdScannerPath(mockConfig);
    assert_string_equal(returnValue, MOCK_SCANNER_PATH);
}
