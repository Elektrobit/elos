// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetInt32_utest.h"

int samconfTestSamconfConfigGetInt32ErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetInt32ErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetInt32ErrorRootNull(UNUSED void **state) {
    int32_t result = 0;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetInt32");
    SHOULD("%s", "return error status since input root is null");

    status = samconfConfigGetInt32(root, testPath, &result);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
