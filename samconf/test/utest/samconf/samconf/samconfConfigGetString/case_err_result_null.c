// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetString_utest.h"

int samconfTestSamconfConfigGetStringErrorResultNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringErrorResultNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringErrorResultNull(UNUSED void **state) {
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

    TEST("samconfConfigGetString");
    SHOULD("%s", "return error status since output result is null");

    status = samconfConfigGetString(&root, testPath, NULL);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
