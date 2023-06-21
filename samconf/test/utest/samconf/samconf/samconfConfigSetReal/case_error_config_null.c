// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigSetReal_utest.h"

int samconfTestSamconfConfigSetRealErrorConfigNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetRealErrorConfigNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetRealErrorConfigNull(UNUSED void **state) {
    double testReal = 49.5609;

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    TEST("samconfConfigSetReal");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since config is null");

    status = samconfConfigSetReal(NULL, testReal);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
