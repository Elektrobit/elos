// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigSetBool_utest.h"

int samconfTestSamconfConfigSetBoolSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetBoolSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetBoolSuccess(UNUSED void **state) {
    bool testValue[] = {true, false};

    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    samconfConfig_t root = {
        .parent = NULL,
        .key = "node",
        .type = SAMCONF_CONFIG_VALUE_STRING,
        .value.string = "test",
        .children = NULL,
        .childCount = 0,
    };

    TEST("samconfConfigSetBool");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since given boolean value is set");

    for (size_t i = 0; i < ARRAY_SIZE(testValue); i++) {
        status = samconfConfigSetBool(&root, testValue[i]);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(root.type, SAMCONF_CONFIG_VALUE_BOOLEAN);
        assert_int_equal(root.value.boolean, testValue[i]);
    }
}
