// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetReal_utest.h"

int samconfTestSamconfConfigGetRealSuccessEntryFoundSetup(void **state) {
    samconfConfigGetRealUtestInit(state);
    return 0;
}

int samconfTestSamconfConfigGetRealSuccessEntryFoundTeardown(void **state) {
    samconfConfigGetRealUtestCleanUp(state);
    return 0;
}

void samconfTestSamconfConfigGetRealSuccessEntryFound(void **state) {
    samconfUteststateT_t *testState = *state;
    double result = 0.0;
    double expectedValue = 3.14;

    const char *testPaths[] = {"Scanner/Syslog/SyslogPath",  "Scanner/Syslog", "Scanner",  "logger", "ip",
                               "/Scanner/Syslog/SyslogPath", "Scanner/",       "/Scanner/"};
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    testState->childrenData[0].type = SAMCONF_CONFIG_VALUE_REAL;
    testState->childrenData[0].value.real = 3.14;
    testState->childrenData[1].type = SAMCONF_CONFIG_VALUE_REAL;
    testState->childrenData[1].value.real = 3.14;
    testState->childrenData[2].type = SAMCONF_CONFIG_VALUE_REAL;
    testState->childrenData[2].value.real = 3.14;
    testState->childrenData[3].type = SAMCONF_CONFIG_VALUE_REAL;
    testState->childrenData[3].value.real = 3.14;
    testState->childrenData[4].type = SAMCONF_CONFIG_VALUE_REAL;
    testState->childrenData[4].value.real = 3.14;
    testState->childrenData[5].type = SAMCONF_CONFIG_VALUE_REAL;
    testState->childrenData[5].value.real = 3.14;

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
        .type = SAMCONF_CONFIG_VALUE_REAL,
        .value.integer = 3.56,
        .children = elosChildren,
        .childCount = 3,
    };

    TEST("samconfConfigGetReal");
    SHOULD("%s", "return OK status since all parameters are correct");

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        status = samconfConfigGetReal(&root, testPaths[i], &result);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_double_equal(result, expectedValue, 0.0);
    }
}
