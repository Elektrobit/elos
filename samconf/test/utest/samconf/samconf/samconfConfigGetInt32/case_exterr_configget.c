// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigGetInt32_utest.h"

int samconfTestSamconfConfigGetInt32ExtErrConfigGetSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32ExtErrConfigGetTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32ExtErrConfigGet(void **state) {
    samconfUteststateT_t *testState = *state;
    int32_t resultInt = 0;

    const char *testPath = "Scanner/Syslog/SyslogPath";
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
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR got from samconfConfigGet ");

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
    expect_value(samconfConfigGet, root, &root);
    expect_value(samconfConfigGet, path, testPath);
    expect_any(samconfConfigGet, result);

    will_return(samconfConfigGet, NULL);
    will_return(samconfConfigGet, SAMCONF_CONFIG_ERROR);

    status = samconfConfigGetInt32(&root, testPath, &resultInt);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_int_equal(resultInt, 0);
}
