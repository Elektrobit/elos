// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safu/mock_safu.h"
#include "samconfConfigNew_utest.h"

int samconfTestSamconfConfigNewAllocMemErrorSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigNewAllocMemErrorTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigNewAllocMemError(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;
    samconfConfig_t *testConfig;

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(samconfConfig_t));
    will_return(__wrap_safuAllocMem, NULL);

    TEST("samconfConfigNew");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR status since memory allocation fails");

    result = samconfConfigNew(&testConfig);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_null(testConfig);
}
