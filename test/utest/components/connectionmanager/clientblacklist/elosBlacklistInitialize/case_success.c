// SPDX-License-Identifier: MIT

#include "elosBlacklistInitialize_utest.h"

int elosTestElosBlacklistInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistInitializeSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistInitializeSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventFilter_t testFilter;
    samconfConfig_t mockConfig = elosGetMockConfig();
    const char *expectedValue = ".event.messageCode 2000 EQ";

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "initialize connection blacklist successfully");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetString, 0);
    expect_value(__wrap_samconfConfigGetString, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetString, path, ELOS_CONFIG_ROOT "EventBlacklist");
    expect_any(__wrap_samconfConfigGetString, result);
    will_set_parameter(__wrap_samconfConfigGetString, result, expectedValue);
    will_return(__wrap_samconfConfigGetString, SAMCONF_CONFIG_OK);

    result = elosBlacklistInitialize(&testFilter, &mockConfig);

    assert_int_equal(result, SAFU_RESULT_OK);

    elosBlacklistDelete(&testFilter);
}
