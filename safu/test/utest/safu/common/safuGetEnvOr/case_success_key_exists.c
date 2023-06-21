// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuGetEnvOr_utest.h"

int safuTestSafuGetEnvOrSuccessKeyExistsSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuGetEnvOrSuccessKeyExistsTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuGetEnvOrSuccessKeyExists(UNUSED void **state) {
    const char *key = "A_ENV_VAR_NAME";
    const char *expectedValue = "A_ENV_VAR_VALUE";

    TEST("safuGetEnvOr");
    SHOULD("%s", "return a pointer to the value of aissigned to an environment variable");

    MOCK_FUNC_AFTER_CALL(getenv, 0);

    expect_value(__wrap_getenv, name, key);
    will_return(__wrap_getenv, expectedValue);

    const char *value = safuGetEnvOr(key, "This value is an error");

    assert_string_equal(expectedValue, value);
}
