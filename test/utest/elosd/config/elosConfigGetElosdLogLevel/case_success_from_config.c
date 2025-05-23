// SPDX-License-Identifier: MIT

#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/log.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
#include <samconf/test_utils.h>

#include "elosConfigGetElosdLogLevel_utest.h"

#define MOCK_CONFIG \
    "{                                    \
        \"root\": {                       \
            \"elos\": {                   \
                \"UseEnv\": false,        \
                \"LogLevel\": \"Verbose\" \
            }                             \
        }                                 \
    }"

int elosTestElosConfigGetElosdLogLevelSuccessFromConfigSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(MOCK_CONFIG, true, config);
    *state = config;
    return 0;
}

int elosTestElosConfigGetElosdLogLevelSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestElosConfigGetElosdLogLevelSuccessFromConfig(void **state) {
    safuLogLevelE_t expectedValue = SAFU_LOG_LEVEL_ALL;
    samconfConfig_t *mockConfig = *(samconfConfig_t **)state;

    TEST("elosConfigGetElosdLogLevel");
    SHOULD("%s", "get the elos LogLevel from the configuration");
    safuLogLevelE_t returnValue = elosConfigGetElosdLogLevel(mockConfig);
    assert_int_equal(returnValue, expectedValue);
}
