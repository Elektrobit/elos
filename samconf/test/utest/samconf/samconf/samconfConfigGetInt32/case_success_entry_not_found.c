// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetInt32_utest.h"

int samconfTestSamconfConfigGetInt32SuccessEntryNotFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32SuccessEntryNotFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32SuccessEntryNotFound(void **state) {
    samconfUteststateT_t *testState = *state;
    int result = 0;
    const char *testPaths[] = {"",
                               "elos/Scanner/Syslog",
                               "Scanner/Syslog/SyslogPath/NotExisting",
                               "Scanner/Syslog/NotExisting",
                               "NotExisting",
                               "NotExisting/"};
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t *syslogChildren[] = {
        &testState->childrenData[0],
        &testState->childrenData[1],
    };

    testState->childrenData[2].children = syslogChildren;
    testState->childrenData[2].childCount = 2;

    samconfConfig_t *scannerChildren[] = {
        &testState->childrenData[2],
    };

    testState->childrenData[4].children = scannerChildren;
    testState->childrenData[4].childCount = 1;

    samconfConfig_t *elosChildren[] = {
        &testState->childrenData[3],
        &testState->childrenData[4],
        &testState->childrenData[5],
    };

    samconfConfig_t root = {
        .parent = NULL,
        .key = "elos",
        .type = SAMCONF_CONFIG_VALUE_INT,
        .value.integer = 36,
        .children = elosChildren,
        .childCount = 3,
    };

    TEST("samconfConfigGetInt32");
    SHOULD("%s", "return config not found since target entry specified in path does not exist");

    for (size_t h = 0; h < ARRAY_SIZE(testPaths); h++) {
        status = samconfConfigGetInt32(&root, testPaths[h], &result);
        assert_int_equal(status, SAMCONF_CONFIG_NOT_FOUND);
        assert_int_equal(result, 0);
    }
}
