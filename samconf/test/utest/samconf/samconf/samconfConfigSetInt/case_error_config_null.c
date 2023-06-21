// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigSetInt_utest.h"

int samconfTestSamconfConfigSetIntErrorConfigNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetIntErrorConfigNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetIntErrorConfigNull(UNUSED void **state) {
    int64_t testInt = 0xDD4FC313B37128B6;

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    TEST("samconfConfigSetInt");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since config is null");

    status = samconfConfigSetInt(NULL, testInt);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
