// SPDX-License-Identifier: MIT

#include <elos/libelosplugin/clientblacklist.h>
#include <elos/rpnfilter/rpnfilter_types.h>
#include <safu/result.h>
#include <samconf/test_utils.h>

#include "elosBlacklistInitialize_utest.h"

static const struct _testTable {
    const char *title;
    const char *config;
} elosTestTable[] = {
    {
        .title = "Blacklist is empty",
        .config = "{"
                  "  \"Config\":{"
                  "    \"EventBlacklist\":\"\""
                  "  }"
                  "}",
    },
    {
        .title = "Blacklist is missing",
        .config = "{"
                  "  \"Config\":{"
                  "  }"
                  "}",
    },
    {
        .title = "Blacklist is has wrong type",
        .config = "{"
                  "  \"Config\":{"
                  "    \"EventBlacklist\": []"
                  "  }"
                  "}",
    },
    {
        .title = "Empty config",
        .config = "{}",
    },
};

int elosTestElosBlacklistInitializeExtErrConfigGetStringSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistInitializeExtErrConfigGetStringTeardown(UNUSED void **state) {
    return 0;
}

static void _testConfig(const char *paramTitle, const char *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventFilter_t testFilter = {0};
    samconfConfig_t mockConfig = {0};

    PARAM("%s: %s", paramTitle, config);
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(config, true, &mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    result = elosBlacklistInitialize(&testFilter, &mockConfig);
    elosRpnFilterStateE_t state = testFilter.state;

    samconfConfigDeleteMembers(&mockConfig);
    elosBlacklistDelete(&testFilter);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_not_equal(state, RPNFILTER_FINALIZED);
}

void elosTestElosBlacklistInitializeExtErrConfigGetString(UNUSED void **state) {
    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not create blacklist filter since config get string fails");

    for (size_t i = 0; i < ARRAY_SIZE(elosTestTable); i++) {
        _testConfig(elosTestTable[i].title, elosTestTable[i].config);
    }
}
