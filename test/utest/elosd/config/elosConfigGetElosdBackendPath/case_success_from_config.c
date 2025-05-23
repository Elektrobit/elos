// SPDX-License-Identifier: MIT

#include <samconf/test_utils.h>

#include "elosConfigGetElosdBackendPath_utest.h"

#define MOCK_BACKEND_PATH "something.c"
#define MOCK_CONFIG                                             \
    "{                                                          \
        \"root\": {                                             \
            \"elos\": {                                         \
                \"UseEnv\": false,                              \
                \"EventLogging\": {                             \
                    \"PluginSearchPath\": \"" MOCK_BACKEND_PATH \
    "\" \
                }                                               \
            }                                                   \
        }                                                       \
    }"

int elosTestElosConfigGetElosdBackendPathSuccessFromConfigSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(MOCK_CONFIG, true, config);

    *state = config;
    return 0;
}

int elosTestElosConfigGetElosdBackendPathSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestElosConfigGetElosdBackendPathSuccessFromConfig(void **state) {
    samconfConfig_t *mockConfig = *(samconfConfig_t **)state;

    TEST("elosConfigGetElosdBackendPath");
    SHOULD("%s", "get the elos backend path environment option");

    const char *returnValue = elosConfigGetElosdBackendPath(mockConfig);
    assert_string_equal(returnValue, MOCK_BACKEND_PATH);
}
