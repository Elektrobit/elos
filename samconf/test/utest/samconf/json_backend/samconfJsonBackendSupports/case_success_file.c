// SPDX-License-Identifier: MIT

#include <samconf/json_backend.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "cmocka_mocks/mock_libc.h"
#include "samconfJsonBackendSupports_utest.h"

int samconfTestSamconfJsonBackendSupportsFileSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfJsonBackendSupportsFileSuccessTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfJsonBackendSupportsFileSuccess(UNUSED void **state) {
    bool testValue = false;
    const char *path[] = {"test.json",      "123.json", "__test.json", "@_%&.json",
                          "18_log:23.json", ".json",    "test.JSON",   "test.JsOn"};

    samconfConfigStatusE_t result = SAMCONF_CONFIG_ERROR;

    TEST("samconfJsonBackendSupports");
    SHOULD("%s", "return true since file extension json is supported");

    for (size_t i = 0; i < ARRAY_SIZE(path); i++) {
        result = samconfJsonBackendSupports(path[i], &testValue);
        assert_int_equal(result, SAMCONF_CONFIG_OK);
        assert_true(testValue);
    }
}
