// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetString_utest.h"

int samconfTestSamconfConfigGetStringSuccessEntryFoundSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringSuccessEntryFoundTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringSuccessEntryFound(void **state) {
    samconfUteststateT_t *testState = *state;
    const char *resultString = NULL;
    const char *expectedValue = "String";

    const char *testPaths[] = {"Scanner/Syslog/SyslogPath",  "Scanner/Syslog", "Scanner",  "logger", "ip",
                               "/Scanner/Syslog/SyslogPath", "Scanner/",       "/Scanner/"};
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    testState->childrenData[1].type = SAMCONF_CONFIG_VALUE_STRING;
    testState->childrenData[1].value.string = "String";

    samconfConfig_t *syslogChildren[] = {
        &testState->childrenData[0],
        &testState->childrenData[1],
    };

    testState->childrenData[2].type = SAMCONF_CONFIG_VALUE_STRING;
    testState->childrenData[2].value.string = "String";
    testState->childrenData[2].children = syslogChildren;
    testState->childrenData[2].childCount = 2;

    samconfConfig_t *scannerChildren[] = {
        &testState->childrenData[2],
    };

    testState->childrenData[3].type = SAMCONF_CONFIG_VALUE_STRING;
    testState->childrenData[3].value.string = "String";
    testState->childrenData[4].type = SAMCONF_CONFIG_VALUE_STRING;
    testState->childrenData[4].value.string = "String";
    testState->childrenData[4].children = scannerChildren;
    testState->childrenData[4].childCount = 1;
    testState->childrenData[5].type = SAMCONF_CONFIG_VALUE_STRING;
    testState->childrenData[5].value.string = "String";

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

    samconfConfig_t *expectedNodes[] = {
        &testState->childrenData[1], &testState->childrenData[2], &testState->childrenData[4],
        &testState->childrenData[3], &testState->childrenData[5], &testState->childrenData[1],
        &testState->childrenData[4], &testState->childrenData[4],

    };

    TEST("samconfConfigGetString");
    SHOULD("%s", "return OK status since all parameters are correct");

    for (size_t i = 0; i < ARRAY_SIZE(testPaths); i++) {
        MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
        expect_value(samconfConfigGet, root, &root);
        expect_value(samconfConfigGet, path, testPaths[i]);
        expect_any(samconfConfigGet, result);

        will_return(samconfConfigGet, expectedNodes[i]);
        will_return(samconfConfigGet, SAMCONF_CONFIG_OK);

        status = samconfConfigGetString(&root, testPaths[i], &resultString);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_string_equal(resultString, expectedValue);
    }
}
