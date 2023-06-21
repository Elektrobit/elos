// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigSetString_utest.h"

int samconfTestSamconfConfigSetStringErrorValueNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetStringErrorValueNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetStringErrorValueNull(UNUSED void **state) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t root = {
        .parent = NULL,
        .key = "elos",
        .type = SAMCONF_CONFIG_VALUE_INT,
        .value.integer = 36,
        .children = NULL,
        .childCount = 0,
    };

    TEST("samconfConfigSetString");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since string value is null");

    status = samconfConfigSetString(&root, NULL);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
