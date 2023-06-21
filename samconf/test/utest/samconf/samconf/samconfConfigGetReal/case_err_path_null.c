// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetReal_utest.h"

int samconfTestSamconfConfigGetRealErrorPathNullSetup(void **state) {
    samconfConfigGetRealUtestInit(state);
    return 0;
}

int samconfTestSamconfConfigGetRealErrorPathNullTeardown(void **state) {
    samconfConfigGetRealUtestCleanUp(state);
    return 0;
}

void samconfTestSamconfConfigGetRealErrorPathNull(void **state) {
    samconfUteststateT_t *testState = *state;
    double result = 0.0;
    const char *testPath = NULL;

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t *elosChildren[] = {
        &testState->childrenData[3],
        &testState->childrenData[4],
        &testState->childrenData[5],
    };

    samconfConfig_t root = {
        .parent = NULL,
        .key = "elos",
        .type = SAMCONF_CONFIG_VALUE_REAL,
        .value.integer = 3.14,
        .children = elosChildren,
        .childCount = 3,
    };

    TEST("samconfConfigGetReal");
    SHOULD("%s", "return error status since input path is null");

    status = samconfConfigGetReal(&root, testPath, &result);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
