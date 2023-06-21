// SPDX-License-Identifier: MIT
#include "samconfConfigGetReal_utest.h"

#include "safu/common.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigGetRealUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigGetRealSuccessEntryFound),
                                       TEST_CASE(samconfTestSamconfConfigGetRealSuccessEntryNotFound),
                                       TEST_CASE(samconfTestSamconfConfigGetRealSuccessNodeNotReal),
                                       TEST_CASE(samconfTestSamconfConfigGetRealExtErrConfigGet),
                                       TEST_CASE(samconfTestSamconfConfigGetRealErrorRootNull),
                                       TEST_CASE(samconfTestSamconfConfigGetRealErrorPathNull),
                                       TEST_CASE(samconfTestSamconfConfigGetRealErrorResultNull)};

    return RUN_TEST_SUITE(tests, samconfConfigGetRealUtest);
}

int samconfConfigGetRealUtestInit(void **state) {
    samconfUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));

    assert_non_null(testState);

    samconfConfig_t configData[] = {
        {
            .parent = NULL,
            .key = "InputFilter",
            .type = SAMCONF_CONFIG_VALUE_INT,
            .value.integer = 38,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "SyslogPath",
            .type = SAMCONF_CONFIG_VALUE_INT,
            .value.integer = 38,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "Syslog",
            .type = SAMCONF_CONFIG_VALUE_INT,
            .value.integer = 38,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "logger",
            .type = SAMCONF_CONFIG_VALUE_INT,
            .value.integer = 38,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "Scanner",
            .type = SAMCONF_CONFIG_VALUE_INT,
            .value.integer = 38,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "ip",
            .type = SAMCONF_CONFIG_VALUE_INT,
            .value.integer = 38,
            .children = NULL,
            .childCount = 0,
        },
    };

    testState->childrenData = safuAllocMem(NULL, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    memcpy(testState->childrenData, configData, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    *state = testState;

    return 0;
}

int samconfConfigGetRealUtestCleanUp(void **state) {
    samconfUteststateT_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}

static int samconfConfigGetRealUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigGetRealUtestTeardown(UNUSED void **state) {
    return 0;
}
