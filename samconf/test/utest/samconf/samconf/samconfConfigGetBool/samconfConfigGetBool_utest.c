// SPDX-License-Identifier: MIT
#include "samconfConfigGetBool_utest.h"

#include "safu/common.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigGetBoolUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigGetBoolSuccessEntryFound),
                                       TEST_CASE(samconfTestSamconfConfigGetBoolSuccessEntryNotFound),
                                       TEST_CASE(samconfTestSamconfConfigGetBoolSuccessNodeNotBool),
                                       TEST_CASE(samconfTestSamconfConfigGetBoolExtErrConfigGet),
                                       TEST_CASE(samconfTestSamconfConfigGetBoolErrorRootNull),
                                       TEST_CASE(samconfTestSamconfConfigGetBoolErrorPathNull),
                                       TEST_CASE(samconfTestSamconfConfigGetBoolErrorResultNull)};

    return RUN_TEST_SUITE(tests, samconfConfigGetBoolUtest);
}

static int samconfConfigGetBoolUtestSetup(void **state) {
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

static int samconfConfigGetBoolUtestTeardown(void **state) {
    samconfUteststateT_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}
