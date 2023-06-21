// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetReal_utest.h"

int samconfTestSamconfConfigGetRealErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetRealErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetRealErrorRootNull(UNUSED void **state) {
    double result = 0.0;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetReal");
    SHOULD("%s", "return error status since input root is null");

    status = samconfConfigGetReal(root, testPath, &result);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
