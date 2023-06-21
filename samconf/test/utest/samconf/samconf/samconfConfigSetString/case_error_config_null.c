// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigSetString_utest.h"

int samconfTestSamconfConfigSetStringErrorConfigNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetStringErrorConfigNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetStringErrorConfigNull(UNUSED void **state) {
    const char *testString = "value";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    TEST("samconfConfigSetString");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since config is null");

    status = samconfConfigSetString(NULL, testString);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
