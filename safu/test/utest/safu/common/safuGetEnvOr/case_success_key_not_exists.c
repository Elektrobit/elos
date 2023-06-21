// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuGetEnvOr_utest.h"

int safuTestSafuGetEnvOrSuccessKeyNotExistsSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuGetEnvOrSuccessKeyNotExistsTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuGetEnvOrSuccessKeyNotExists(UNUSED void **state) {
    const char *key = "A_ENV_VAR_NAME";
    const char *expectedValue = "A_ENV_VAR_VALUE";

    TEST("safuGetEnvOr");
    SHOULD("%s", "return a pointer to the provided default value");

    MOCK_FUNC_AFTER_CALL(getenv, 0);

    expect_value(__wrap_getenv, name, key);
    will_return(__wrap_getenv, NULL);

    const char *value = safuGetEnvOr(key, expectedValue);

    assert_string_equal(expectedValue, value);
}
