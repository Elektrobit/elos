// SPDX-License-Identifier: MIT
#include <cmocka_mocks/mock_libc.h>
#include <stdint.h>

#include "samconfConfigSetString_utest.h"

int samconfTestSamconfConfigSetStringExtErrStrdupSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetStringExtErrStrdupTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetStringExtErrStrdup(UNUSED void **state) {
    const char *testString = "value";

    samconfConfigStatusE_t status = SAMCONF_CONFIG_OK;

    samconfConfig_t root = {
        .parent = NULL,
        .key = "elos",
        .type = SAMCONF_CONFIG_VALUE_INT,
        .value.integer = 36,
        .children = NULL,
        .childCount = 0,
    };

    TEST("samconfConfigSetString");
    SHOULD("%s", "return SAMCONF_CONFIG_ERROR since stdup returns error");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_string(__wrap_strdup, string, testString);
    will_return(__wrap_strdup, NULL);

    status = samconfConfigSetString(&root, testString);
    assert_int_equal(status, SAMCONF_CONFIG_ERROR);
}
