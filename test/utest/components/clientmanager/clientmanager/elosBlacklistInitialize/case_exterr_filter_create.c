// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>

#include "elosBlacklistInitialize_utest.h"
#include "mock_eventfilter.h"

int elosTestElosBlacklistInitializeErrFilterCreateSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistInitializeErrFilterCreateTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistInitializeErrFilterCreate(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventFilter_t testFilter;
    samconfConfig_t mockConfig = elosGetMockConfig();
    const char *expectedValue = ".event.messageCode 2000 EQ";

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not initialize blacklist since filter creation fails");

    MOCK_FUNC_AFTER_CALL(samconfConfigGetString, 0);
    expect_value(__wrap_samconfConfigGetString, root, &mockConfig);
    expect_string(__wrap_samconfConfigGetString, path, ELOS_CONFIG_ROOT "EventBlacklist");
    expect_any(__wrap_samconfConfigGetString, result);
    will_set_parameter(__wrap_samconfConfigGetString, result, expectedValue);
    will_return(__wrap_samconfConfigGetString, SAMCONF_CONFIG_OK);

    MOCK_FUNC_AFTER_CALL(elosEventFilterCreate, 0);
    expect_any(elosEventFilterCreate, filter);
    expect_any(elosEventFilterCreate, param);
    will_set_parameter(elosEventFilterCreate, filter, &testFilter);
    will_return(elosEventFilterCreate, RPNFILTER_RESULT_ERROR);

    result = elosBlacklistInitialize(&testFilter, &mockConfig);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
