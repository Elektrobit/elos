// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetBool_utest.h"

int samconfTestSamconfConfigGetBoolErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetBoolErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetBoolErrorRootNull(UNUSED void **state) {
    bool result = false;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetBool");
    SHOULD("%s", "return error status since input root is null");

    status = samconfConfigGetBool(root, testPath, &result);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
