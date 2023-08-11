// SPDX-License-Identifier: MIT
#include "elosAuthorizedProcessDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosAuthorizedProcessDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosAuthorizedProcessDeleteSuccess),
        TEST_CASE(elosTestElosAuthorizedProcessDeleteAuthorizedprocessParamNull),
    };
    return RUN_TEST_SUITE(tests, elosAuthorizedProcessDeleteUtest);
}

int elosAuthorizedProcessDeleteUtestInit(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));

    samconfConfig_t configData[] = {
        {
            .parent = NULL,
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = ".process.uid 0 EQ .process.gid 0 EQ .process.exec '/bin/elosc' STRCMP AND",
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = ".process.gid 200 EQ .process.exec '/bin/elosc' STRCMP AND",
            .children = NULL,
            .childCount = 0,
        },
        {
            .parent = NULL,
            .type = SAMCONF_CONFIG_VALUE_STRING,
            .value.string = ".process.pid 0 EQ",
            .children = NULL,
            .childCount = 0,
        },

    };

    testState->childrenData = safuAllocMem(NULL, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    memcpy(testState->childrenData, configData, ARRAY_SIZE(configData) * sizeof(*testState->childrenData));

    *state = testState;

    return 0;
}

int elosAuthorizedProcessDeleteUtestCleanUp(void **state) {
    elosUteststateT_t *testState = *state;

    free(testState->childrenData);
    free(testState);

    return 0;
}

static int elosAuthorizedProcessDeleteUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosAuthorizedProcessDeleteUtestTeardown(UNUSED void **state) {
    return 0;
}
