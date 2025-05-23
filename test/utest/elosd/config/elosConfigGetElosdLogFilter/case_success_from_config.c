// SPDX-License-Identifier: MIT

#include <samconf/json_backend.h>
#include <samconf/samconf.h>
#include <samconf/test_utils.h>

#include "elosConfigGetElosdLogFilter_utest.h"

#define MOCK_FILTER "something.c"
#define MOCK_CONFIG                            \
    "{                                         \
        \"root\": {                            \
            \"elos\": {                        \
                \"UseEnv\": false,             \
                \"LogFilter\": \"" MOCK_FILTER \
    "\" \
            }                                  \
        }                                      \
    }"

int elosTestElosConfigGetElosdLogFilterSuccessFromConfigSetup(void **state) {
    samconfConfig_t *config = NULL;
    samconfConfigNew(&config);
    samconfUtilCreateMockConfigFromStr(MOCK_CONFIG, true, config);

    *state = config;
    return 0;
}

int elosTestElosConfigGetElosdLogFilterSuccessFromConfigTeardown(void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    *state = NULL;
    return 0;
}

void elosTestElosConfigGetElosdLogFilterSuccessFromConfig(void **state) {
    samconfConfig_t *mockConfig = *(samconfConfig_t **)state;

    TEST("elosConfigGetElosdLogFilter");
    SHOULD("%s", "get the elos logFilter from the config");

    const char *returnValue = elosConfigGetElosdLogFilter(mockConfig);
    assert_string_equal(returnValue, MOCK_FILTER);
}
