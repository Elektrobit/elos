// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetBool_utest.h"

int samconfTestSamconfConfigGetBoolErrorResultNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolErrorResultNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolErrorResultNull(UNUSED void **state) {
    samconfUteststateT_t *testState = *state;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

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
    SHOULD("%s", "return error status since output result is null");

    status = samconfConfigGetBool(&root, testPath, NULL);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
