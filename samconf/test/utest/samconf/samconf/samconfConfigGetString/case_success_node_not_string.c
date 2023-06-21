// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetString_utest.h"

int samconfTestSamconfConfigGetStringSuccessNodeNotStringSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringSuccessNodeNotStringTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringSuccessNodeNotString(void **state) {
    samconfUteststateT_t *testState = *state;
    const char *resultString = NULL;

    const char *testPath = "Scanner/Syslog/SyslogPath";
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    testState->childrenData[1].type = SAMCONF_CONFIG_VALUE_INT;
    testState->childrenData[1].value.integer = 48;
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

    samconfConfig_t *expectedNodes[] = {
        &testState->childrenData[1],
    };

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);

    TEST("samconfConfigGetString");
    SHOULD("%s", "return OK status but no string value returned");

    expect_value(samconfConfigGet, root, &root);
    expect_value(samconfConfigGet, path, testPath);
    expect_any(samconfConfigGet, result);

    will_return(samconfConfigGet, expectedNodes[0]);
    will_return(samconfConfigGet, SAMCONF_CONFIG_OK);

    status = samconfConfigGetString(&root, testPath, &resultString);
    assert_int_equal(status, SAMCONF_CONFIG_INVALID_TYPE);
    assert_null(resultString);
}
