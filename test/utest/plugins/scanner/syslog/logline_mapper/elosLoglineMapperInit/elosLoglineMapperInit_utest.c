// SPDX-License-Identifier: MIT
#include "elosLoglineMapperInit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLoglineMapperInitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLoglineMapperInitSuccess),
        TEST_CASE(elosTestElosLoglineMapperInitSectionedMessageCodes),
        TEST_CASE(elosTestElosLoglineMapperInitErrDeepNestedMessageCodes),
        TEST_CASE(elosTestElosLoglineMapperInitNullConfig),
    };

    return RUN_TEST_SUITE(tests, elosLoglineMapperInitUtest);
}

int elosLoglineMapperInitUtestInit(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));

    samconfConfig_t configData[] = {
        {
            .parent = NULL,
            .key = "MappingRules",
            .type = SAMCONF_CONFIG_VALUE_OBJECT,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "MessageCodes",
            .type = SAMCONF_CONFIG_VALUE_OBJECT,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "sshd",
            .type = SAMCONF_CONFIG_VALUE_OBJECT,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "crinit",
            .type = SAMCONF_CONFIG_VALUE_OBJECT,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "login",
            .type = SAMCONF_CONFIG_VALUE_OBJECT,
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "4000",
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = ".event.source.appName 'ssh' STRCMP",
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "2000",
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = ".event.source.appName 'crinit' STRCMP",
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "1000",
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = ".event.source.appName 'login' STRCMP",
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "5000",
            .type = SAMCONF_CONFIG_VALUE_OBJECT,
            .children = NULL,
            .childCount = 0,
        },
    };

    testState->childrenData = safuAllocMem(NULL, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    memcpy(testState->childrenData, configData, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    *state = testState;

    return 0;
}

int elosLoglineMapperInitUtestCleanUp(void **state) {
    elosUteststateT_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}

static int elosLoglineMapperInitUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLoglineMapperInitUtestTeardown(UNUSED void **state) {
    return 0;
}
