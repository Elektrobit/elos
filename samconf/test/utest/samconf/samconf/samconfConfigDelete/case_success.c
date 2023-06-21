// SPDX-License-Identifier: MIT
#include <samconf/samconf.h>
#include <stdint.h>
#include <stdlib.h>

#include "mock_samconf.h"
#include "samconfConfigDelete_utest.h"

int samconfTestSamconfConfigDeleteSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigDeleteSuccess(UNUSED void **state) {
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *testConfig = NULL;
    status = samconfConfigNew(&testConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    status = samconfConfigDelete(testConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
}
