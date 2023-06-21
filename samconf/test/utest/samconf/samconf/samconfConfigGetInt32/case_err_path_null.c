// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetInt32_utest.h"

int samconfTestSamconfConfigGetInt32ErrorPathNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32ErrorPathNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32ErrorPathNull(UNUSED void **state) {
    samconfUteststateT_t *testState = *state;
    int32_t result = 0;
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
        .type = SAMCONF_CONFIG_VALUE_INT,
        .value.integer = 36,
        .children = elosChildren,
        .childCount = 3,
    };

    TEST("samconfConfigGetInt32");
    SHOULD("%s", "return error status since input path is null");

    status = samconfConfigGetInt32(&root, testPath, &result);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
