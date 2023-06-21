// SPDX-License-Identifier: MIT
#include "samconfConfigGet_utest.h"

#include "safu/common.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigGetUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigGetErrorNullResult),
                                       TEST_CASE(samconfTestSamconfConfigGetErrorNullRoot),
                                       TEST_CASE(samconfTestSamconfConfigGetErrorRootHasNoChild),
                                       TEST_CASE(samconfTestSamconfConfigGetSuccessEntryFound),
                                       TEST_CASE(samconfTestSamconfConfigGetSuccessEntryNotFound),
                                       TEST_CASE(samconfTestSamconfConfigGetSuccessResultNonNull),
                                       TEST_CASE(samconfTestSamconfConfigGetSuccessRootChildCountZero),
                                       TEST_CASE(samconfTestSamconfConfigGetExtErrorStrdup),
                                       TEST_CASE(samconfTestSamconfConfigGetErrorNullPath)};

    return RUN_TEST_SUITE(tests, samconfConfigGetUtest);
}

int samconfConfigGetUtestInit(void **state) {
    samconfUteststate_t *testState = safuAllocMem(NULL, sizeof(*testState));

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

    assert_non_null(testState->childrenData);

    memcpy(testState->childrenData, configData, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    *state = testState;

    return 0;
}

int samconfConfigGetUtestCleanup(void **state) {
    samconfUteststate_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}

static int samconfConfigGetUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigGetUtestTeardown(UNUSED void **state) {
    return 0;
}
