// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>
#include <sys/stat.h>

#include "elosConfigLoad_utest.h"

int elosTestElosConfigLoadInvalidConfigPathErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigLoadInvalidConfigPathErrorTeardown(UNUSED void **state) {
    return 0;
}

int elosTestDummyStat(UNUSED const char *pathname, struct stat *statbuf) {
    statbuf->st_mode = 0x1000;
    return 0;
}

void elosTestElosConfigLoadInvalidConfigPathError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    samconfConfig_t *testConfig = NULL;

    TEST("elosConfigLoad");
    SHOULD("return error since config path is invalid");

    MOCK_FUNC_AFTER_CALL(safuGetEnvOr, 0);
    expect_string(__wrap_safuGetEnvOr, key, "ELOS_CONFIG_PATH");
    expect_any(__wrap_safuGetEnvOr, defaultValue);
    will_return(__wrap_safuGetEnvOr, NULL);

    MOCK_FUNC_AFTER_CALL_WITH(stat, 0, elosTestDummyStat, NULL);

    result = elosConfigLoad(&testConfig);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
