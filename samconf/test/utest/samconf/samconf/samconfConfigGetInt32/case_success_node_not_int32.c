// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetInt32_utest.h"

int samconfTestSamconfConfigGetInt32SuccessNodeNotInt32Setup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32SuccessNodeNotInt32Teardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32SuccessNodeNotInt32(void **state) {
    samconfUteststateT_t *testState = *state;
    int32_t resultInt = 0;

    const char *testPath = "Scanner/Syslog/SyslogPath";
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    testState->childrenData[1].type = SAMCONF_CONFIG_VALUE_STRING;
    testState->childrenData[1].value.string = "string";
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
    SHOULD("%s", "return Invalid type status as no int32 value returned");

    status = samconfConfigGetInt32(&root, testPath, &resultInt);
    assert_int_equal(status, SAMCONF_CONFIG_INVALID_TYPE);
    assert_int_equal(resultInt, 0);
}
