// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <elos/libelosplugin/clientblacklist.h>
#include <elos/rpnfilter/rpnfilter_types.h>
#include <safu/result.h>
#include <samconf/test_utils.h>

#include "elosBlacklistInitialize_utest.h"
#include "mock_eventfilter.h"

int elosTestElosBlacklistInitializeErrFilterCreateSetup(void **state) {
    elosTestState_t *testState = *state;

    memset(testState, 0, sizeof(elosTestState_t));
    samconfConfigStatusE_t status = samconfUtilCreateMockConfigFromStr(
        "{"
        "  \"Config\":{"
        "    \"EventBlacklist\":\".e.messageCode 1 EQ\""
        "  }"
        "}",
        true, &testState->mockConfig);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    return 0;
}

int elosTestElosBlacklistInitializeErrFilterCreateTeardown(void **state) {
    elosTestState_t *testState = *state;
    elosBlacklistDelete(&testState->testFilter);
    samconfConfigDeleteMembers(&testState->mockConfig);
    return 0;
}

void elosTestElosBlacklistInitializeErrFilterCreate(void **state) {
    elosTestState_t *testState = *state;

    TEST("elosBlacklistInitialize");
    SHOULD("%s", "not initialize blacklist since filter creation fails");

    MOCK_FUNC_AFTER_CALL(elosEventFilterCreate, 0);
    expect_value(elosEventFilterCreate, filter, &testState->testFilter);
    expect_any(elosEventFilterCreate, param);
    will_set_parameter(elosEventFilterCreate, filter, &testState->testFilter);
    will_return(elosEventFilterCreate, RPNFILTER_RESULT_ERROR);

    safuResultE_t result = elosBlacklistInitialize(&testState->testFilter, &testState->mockConfig);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
