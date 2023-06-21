// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "samconfConfigGetString_utest.h"

int samconfTestSamconfConfigGetStringErrorRootNullSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigGetStringErrorRootNullTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigGetStringErrorRootNull(UNUSED void **state) {
    const char *result = NULL;
    const char *testPath = "Scanner/Syslog/SyslogPath";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t *root = NULL;

    TEST("samconfConfigGetString");
    SHOULD("%s", "return error status since input root is null");

    status = samconfConfigGetString(root, testPath, &result);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
    assert_null(result);
}
