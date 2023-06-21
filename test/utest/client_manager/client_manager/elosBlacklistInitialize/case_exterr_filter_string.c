// SPDX-License-Identifier: MIT

#include "elosBlacklistInitialize_utest.h"
#include "safu/common.h"
#include "safu/log.h"
#include "safu/mock_log.h"
#include "safu/mock_safu.h"

int elosTestElosBlacklistInitializeExtErrFilterStringSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistInitializeExtErrFilterStringTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistInitializeExtErrFilterString(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventFilter_t testFilter;
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not create blacklist filter since filter string is empty");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetString, 0);
    expect_value(__wrap_samconfConfigGetString, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetString, path, ELOS_CONFIG_ROOT "EventBlacklist");
    expect_any(__wrap_samconfConfigGetString, result);
    will_set_parameter(__wrap_samconfConfigGetString, result, NULL);
    will_return(__wrap_samconfConfigGetString, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(safuLogFunc, 0);
    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "blacklist filter string is not set in config");
    will_return(__wrap_safuLogFunc, SAFU_LOG_STATUS_SUCCESS);

    result = elosBlacklistInitialize(&testFilter, &mockConfig);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_not_equal(testFilter.state, RPNFILTER_FINALIZED);
}
