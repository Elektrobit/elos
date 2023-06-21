// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safu/mock_safu.h"
#include "samconfConfigNew_utest.h"

int samconfTestSamconfConfigNewErrorNullConfigSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigNewErrorNullConfigTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigNewErrorNullConfig(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;

    TEST("samconfConfigNew");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR status since config is NULL");

    result = samconfConfigNew(NULL);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
}
