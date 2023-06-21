// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetBool_utest.h"

int samconfTestSamconfConfigGetBoolSuccessEntryFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolSuccessEntryFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolSuccessEntryFound(void **state) {
    samconfUteststateT_t *testState = *state;
    bool resultBool = false;

    const char *testPaths[] = {"Scanner/Syslog/SyslogPath",  "Scanner/Syslog", "Scanner",  "logger", "ip",
                               "/Scanner/Syslog/SyslogPath", "Scanner/",       "/Scanner/"};
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    testState->childrenData[1].type = SAMCONF_CONFIG_VALUE_BOOLEAN;
    testState->childrenData[1].value.boolean = true;

    samconfConfig_t *syslogChildren[] = {
        &testState->childrenData[0],
        &testState->childrenData[1],
    };

    testState->childrenData[2].type = SAMCONF_CONFIG_VALUE_BOOLEAN;
    testState->childrenData[2].value.boolean = true;
    testState->childrenData[2].children = syslogChildren;
    testState->childrenData[2].childCount = 2;

    samconfConfig_t *scannerChildren[] = {
        &testState->childrenData[2],
    };

    testState->childrenData[3].type = SAMCONF_CONFIG_VALUE_BOOLEAN;
    testState->childrenData[3].value.boolean = true;
    testState->childrenData[4].type = SAMCONF_CONFIG_VALUE_BOOLEAN;
    testState->childrenData[4].value.boolean = true;
    testState->childrenData[4].children = scannerChildren;
    testState->childrenData[4].childCount = 1;
    testState->childrenData[5].type = SAMCONF_CONFIG_VALUE_BOOLEAN;
    testState->childrenData[5].value.boolean = true;

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

    TEST("samconfConfigGetBool");
    SHOULD("%s", "return OK status since all parameters are correct and entry of type boolean found");

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        status = samconfConfigGetBool(&root, testPaths[i], &resultBool);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_true(resultBool);
    }
}
