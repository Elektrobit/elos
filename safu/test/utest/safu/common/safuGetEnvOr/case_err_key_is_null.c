// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuGetEnvOr_utest.h"

int safuTestSafuGetEnvOrErrKeyIsNullSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuGetEnvOrErrKeyIsNullTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuGetEnvOrErrKeyIsNull(UNUSED void **state) {
    const char *expectedValue = "A_ENV_VAR_VALUE";

    TEST("safuGetEnvOr");
    SHOULD("%s", "return a pointer to the default value");

    const char *value = safuGetEnvOr(NULL, expectedValue);

    assert_string_equal(expectedValue, value);
}
