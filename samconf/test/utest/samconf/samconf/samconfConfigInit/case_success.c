// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safu/common.h"
#include "samconfConfigInit_utest.h"

int samconfTestSamconfConfigInitSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigInitSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigInitSuccess(UNUSED void **state) {
    samconfConfigStatusE_t testStatus = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *testConfig = safuAllocMem(NULL, sizeof(*testConfig));

    TEST("samconfConfigInit");
    SHOULD("%s", "return SAMCONF_CONFIG_OK status since input is valid");

    testStatus = samconfConfigInit(testConfig);

    assert_int_equal(testStatus, SAMCONF_CONFIG_OK);
    assert_null(testConfig->key);
    assert_int_equal(0, testConfig->type);
    assert_null(testConfig->children);
    assert_int_equal(0, testConfig->childCount);

    free(testConfig);
}
