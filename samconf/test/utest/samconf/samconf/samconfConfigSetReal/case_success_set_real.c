// SPDX-License-Identifier: MIT
#include <float.h>
#include <limits.h>
#include <stdint.h>

#include "samconfConfigSetReal_utest.h"

int samconfTestSamconfConfigSetRealSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfConfigSetRealSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfConfigSetRealSuccess(UNUSED void **state) {
    double testValue[] = {6.2898, DBL_MAX, DBL_MIN, 0.0, 3.143};

    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;

    samconfConfig_t root = {
        .parent = NULL,
        .key = "node",
        .type = SAMCONF_CONFIG_VALUE_STRING,
        .value.string = "test",
        .children = NULL,
        .childCount = 0,
    };

    TEST("samconfConfigSetReal");
    SHOULD("%s", "return SAMCONF_CONFIG_OK since given real value is set");

    for (size_t i = 0; i < ARRAY_SIZE(testValue); i++) {
        status = samconfConfigSetReal(&root, testValue[i]);
        assert_int_equal(status, SAMCONF_CONFIG_OK);
        assert_int_equal(root.type, SAMCONF_CONFIG_VALUE_REAL);
        assert_float_equal(root.value.real, testValue[i], 0.0);
    }
}
