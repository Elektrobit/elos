// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <stdlib.h>

#include "samconfConfigInit_utest.h"

int samconfTestSamconfConfigInitErrorNullConfigSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigInitErrorNullConfigTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigInitErrorNullConfig(UNUSED void **state) {
    samconfConfigStatusE_t testStatus = SAMCONF_CONFIG_OK;

    testStatus = samconfConfigInit(NULL);

    assert_int_equal(testStatus, SAMCONF_CONFIG_ERROR);
}
