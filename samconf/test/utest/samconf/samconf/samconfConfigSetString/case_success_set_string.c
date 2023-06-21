// SPDX-License-Identifier: MIT
#include <cmocka_mocks/mock_libc.h>
#include <stdint.h>

#include "samconfConfigSetString_utest.h"

int samconfTestSamconfConfigSetStringSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetStringSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetStringSuccess(UNUSED void **state) {
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
    SHOULD("%s", "return SAMCONF_CONFIG_OK since given string value is set");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_string(__wrap_strdup, string, testString);
    will_return(__wrap_strdup, testString);

    status = samconfConfigSetString(&root, testString);
    assert_int_equal(status, SAMCONF_CONFIG_OK);
    assert_int_equal(root.type, SAMCONF_CONFIG_VALUE_STRING);
    assert_string_equal(root.value.string, testString);
}
