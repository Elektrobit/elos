// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigSetBool_utest.h"

int samconfTestSamconfConfigSetBoolErrorConfigNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetBoolErrorConfigNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetBoolErrorConfigNull(UNUSED void **state) {
    bool testBool = false;

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    TEST("samconfConfigSetBool");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since config is null");

    status = samconfConfigSetBool(NULL, testBool);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
