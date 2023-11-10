// SPDX-License-Identifier: MIT
#include <sys/types.h>

#include "elosAuthorizedProcessDelete_utest.h"

int elosTestElosAuthorizedProcessDeleteSuccessSetup(void **state) {
    elosAuthorizedProcessDeleteUtestInit(state);
    return 0;
}

int elosTestElosAuthorizedProcessDeleteSuccessTeardown(void **state) {
    elosAuthorizedProcessDeleteUtestCleanUp(state);
    return 0;
}

void elosTestElosAuthorizedProcessDeleteSuccess(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosUteststateT_t *testState = *state;
    safuVec_t testFilter = {0};

    samconfConfig_t *processFilters[] = {
        &testState->childrenData[0],
        &testState->childrenData[1],
        &testState->childrenData[2],
    };

    samconfConfig_t mockConfig = {
        .parent = NULL,
        .key = "authorizedProcesses",
        .type = SAMCONF_CONFIG_VALUE_ARRAY,
        .children = processFilters,
        .childCount = 3,
    };

    samconfConfig_t mockRoot = {
        .parent = NULL,
        .key = "root",
        .type = SAMCONF_CONFIG_VALUE_OBJECT,
        .children = NULL,
        .childCount = 0,
    };

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
    expect_value(__wrap_samconfConfigGet, root, &mockRoot);
    expect_string(__wrap_samconfConfigGet, path, ELOS_CONFIG_ROOT "authorizedProcesses");
    expect_any(__wrap_samconfConfigGet, result);
    will_set_parameter(__wrap_samconfConfigGet, result, &mockConfig);
    will_return(__wrap_samconfConfigGet, SAMCONF_CONFIG_OK);

    result = elosAuthorizedProcessInitialize(&testFilter, &mockRoot);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testFilter.elementCount, 3);

    TEST("elosAuthorizedProcessDelete");
    SHOULD("%s", "delete authorized process filter successfully");

    result = elosAuthorizedProcessDelete(&testFilter);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(testFilter.elementCount, 0);
}
