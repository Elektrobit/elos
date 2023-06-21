// SPDX-License-Identifier: MIT
#include "samconfConfigGetString_utest.h"

#include "safu/common.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigGetStringUtest)

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(samconfTestSamconfConfigGetStringSuccessEntryFound),
        cmocka_unit_test(samconfTestSamconfConfigGetStringSuccessEntryNotFound),
        cmocka_unit_test(samconfTestSamconfConfigGetStringSuccessNodeNotString),
        cmocka_unit_test(samconfTestSamconfConfigGetStringErrorRootNull),
        cmocka_unit_test(samconfTestSamconfConfigGetStringErrorPathNull),
        cmocka_unit_test(samconfTestSamconfConfigGetStringErrorResultNull),
    };

    return RUN_TEST_SUITE(tests, samconfConfigGetStringUtest);
}

static int samconfConfigGetStringUtestSetup(void **state) {
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

static int samconfConfigGetStringUtestTeardown(void **state) {
    samconfUteststateT_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}
