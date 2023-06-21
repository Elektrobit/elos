// SPDX-License-Identifier: MIT
#include "samconfConfigAdd_utest.h"

#include "safu/common.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigAddUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigAddSuccessChildAdded), TEST_CASE(samconfTestSamconfConfigAddErrorParentNull),
        TEST_CASE(samconfTestSamconfConfigAddErrorChildNull),    TEST_CASE(samconfTestSamconfConfigAddAllocMemError),
        TEST_CASE(samconfTestSamconfConfigAddSuccessEmptyNode),
    };

    return RUN_TEST_SUITE(tests, samconfConfigAddUtest);
}

int samconfConfigAddUtestInit(void **state) {
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

int samconfConfigAddUtestCleanUp(void **state) {
    samconfUteststate_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}

static int samconfConfigAddUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigAddUtestTeardown(UNUSED void **state) {
    return 0;
}
