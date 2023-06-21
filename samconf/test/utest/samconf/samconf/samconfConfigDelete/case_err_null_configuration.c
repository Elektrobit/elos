// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "mock_samconf.h"
#include "samconfConfigDelete_utest.h"

int samconfTestSamconfConfigDeleteNullConfigurationSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigDeleteNullConfigurationTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigDeleteNullConfiguration(UNUSED void **state) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    status = samconfConfigDelete(NULL);

    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
