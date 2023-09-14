// SPDX-License-Identifier: MIT
#include "elosLoglineMapperDoMapping_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosLoglineMapperDoMappingUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLoglineMapperDoMappingSuccess),
    };

    return RUN_TEST_SUITE(tests, elosLoglineMapperDoMappingUtest);
}

int elosLoglineMapperDoMappingUtestInit(void **state) {
    elosTestState_t *testState = safuAllocMem(NULL, sizeof(*testState));

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
            .key = "4000",
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = "': Server listening on 0.0.0.0 port 22.' .event.payload STRCMP ': Server listening on :: "
                            "port 22.' .event.payload STRCMP OR",
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "2000",
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = "\": ROOT LOGIN  on '/dev/console'\" .event.payload STRCMP",
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .key = "1000",
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = "\" hehyoi\" .event.payload STRCMP",
            .children = NULL,
            .childCount = 0,
        },
    };

    testState->childrenData = safuAllocMem(NULL, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    memcpy(testState->childrenData, configData, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    *state = testState;

    return 0;
}

int elosLoglineMapperDoMappingUtestCleanUp(void **state) {
    elosTestState_t *testState = *state;

    elosLoglineMapperDeleteMembers(&testState->mapper);
    free(testState->childrenData);
    free(testState);

    return 0;
}

static int elosLoglineMapperDoMappingUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLoglineMapperDoMappingUtestTeardown(UNUSED void **state) {
    return 0;
}
