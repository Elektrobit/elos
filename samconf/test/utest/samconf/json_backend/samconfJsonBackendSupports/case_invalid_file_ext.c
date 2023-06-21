// SPDX-License-Identifier: MIT

#include <samconf/json_backend.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "cmocka_mocks/mock_libc.h"
#include "samconfJsonBackendSupports_utest.h"

int samconfTestSamconfJsonBackendSupportsNoInvalidExtensionSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfJsonBackendSupportsNoInvalidExtensionTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfJsonBackendSupportsNoInvalidExtension(UNUSED void **state) {
    bool testValue = false;
    const char *path[] = {"test.j123", "123.txt", "__test.j&$n", "@_%&.1239", "18_log:23.", "test"};

    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;

    TEST("samconfJsonBackendSupports no invalid file extension");
    SHOULD("%s", "Return SAMCONF_CONFIG_ERROR");

    for (size_t i = 0; i < ARRAY_SIZE(path); i++) {
        result = samconfJsonBackendSupports(path[i], &testValue);
        assert_int_equal(result, SAMCONF_CONFIG_ERROR);
        assert_false(testValue);
    }
}
