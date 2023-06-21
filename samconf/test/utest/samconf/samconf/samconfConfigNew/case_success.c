// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigNew_utest.h"

int samconfTestSamconfConfigNewSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigNewSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigNewSuccess(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *testConfig;

    TEST("samconfConfigNew");
    SHOULD("%s", "return SAMCONF_CONFIG_OK status since input is valid");

    result = samconfConfigNew(&testConfig);

    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_non_null(testConfig);

    free(testConfig);
}
