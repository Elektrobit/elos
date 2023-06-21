// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetInt32_utest.h"

int samconfTestSamconfConfigGetInt32SuccessEntryFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32SuccessEntryFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32SuccessEntryFound(void **state) {
    samconfUteststateT_t *testState = *state;
    int32_t resultInt = 0;
    int32_t expectedValue = 38;

    const char *testPaths[] = {"Scanner/Syslog/SyslogPath",  "Scanner/Syslog", "Scanner",  "logger", "ip",
                               "/Scanner/Syslog/SyslogPath", "Scanner/",       "/Scanner/"};
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
    SHOULD("%s", "return OK status since all parameters are correct");

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        status = samconfConfigGetInt32(&root, testPaths[i], &resultInt);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(resultInt, expectedValue);
    }
}
