// SPDX-License-Identifier: MIT

#include "elosBlacklistInitialize_utest.h"

int elosTestElosBlacklistInitializeExtErrConfigGetStringSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistInitializeExtErrConfigGetStringTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistInitializeExtErrConfigGetString(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventFilter_t testFilter;
    samconfConfig_t mockConfig = elosGetMockConfig();
    const char *expectedValue = ".event.messageCode 2000 EQ";

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not create blacklist filter since config get string fails");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetString, 0);
    expect_value(__wrap_samconfConfigGetString, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetString, path, ELOS_CONFIG_ROOT "EventBlacklist");
    expect_any(__wrap_samconfConfigGetString, result);
    will_set_parameter(__wrap_samconfConfigGetString, result, expectedValue);
    will_return(__wrap_samconfConfigGetString, SAMCONF_CONFIG_ERROR);

    result = elosBlacklistInitialize(&testFilter, &mockConfig);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_not_equal(testFilter.state, RPNFILTER_FINALIZED);
}
