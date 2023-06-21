// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigSetInt_utest.h"

int samconfTestSamconfConfigSetIntSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetIntSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetIntSuccess(UNUSED void **state) {
    int64_t testValue[] = {0x7F102B892386AF1F, INT64_MAX, 0, INT64_MIN};

    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    samconfConfig_t root = {
        .parent = NULL,
        .key = "node",
        .type = SAMCONF_CONFIG_VALUE_STRING,
        .value.string = "test",
        .children = NULL,
        .childCount = 0,
    };

    TEST("samconfConfigSetInt");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since given integer value is set");

    for (size_t i = 0; i < ARRAY_SIZE(testValue); i++) {
        status = samconfConfigSetInt(&root, testValue[i]);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(root.type, SAMCONF_CONFIG_VALUE_INT);
        assert_int_equal(root.value.integer, testValue[i]);
    }
}
