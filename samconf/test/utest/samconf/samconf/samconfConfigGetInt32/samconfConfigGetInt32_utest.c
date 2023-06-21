// SPDX-License-Identifier: MIT
#include "samconfConfigGetInt32_utest.h"

#include "safu/common.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigGetInt32Utest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigGetInt32SuccessEntryFound),
                                       TEST_CASE(samconfTestSamconfConfigGetInt32SuccessEntryNotFound),
                                       TEST_CASE(samconfTestSamconfConfigGetInt32SuccessNodeNotInt32),
                                       TEST_CASE(samconfTestSamconfConfigGetInt32ExtErrConfigGet),
                                       TEST_CASE(samconfTestSamconfConfigGetInt32ErrorRootNull),
                                       TEST_CASE(samconfTestSamconfConfigGetInt32ErrorPathNull),
                                       TEST_CASE(samconfTestSamconfConfigGetInt32ErrorResultNull)};

    return RUN_TEST_SUITE(tests, samconfConfigGetInt32Utest);
}

static int samconfConfigGetInt32UtestSetup(void **state) {
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

static int samconfConfigGetInt32UtestTeardown(void **state) {
    samconfUteststateT_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}
