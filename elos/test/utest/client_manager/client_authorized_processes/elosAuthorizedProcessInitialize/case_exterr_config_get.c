// SPDX-License-Identifier: MIT

#include <sys/types.h>

#include "elosAuthorizedProcessInitialize_utest.h"

int elosTestElosAuthorizedProcessInitializeExtErrConfigGetSetup(void **state) {
    elosAuthorizedProcessInitUtestInit(state);
    return 0;
}

int elosTestElosAuthorizedProcessInitializeExtErrConfigGetTeardown(void **state) {
    elosAuthorizedProcessInitUtestCleanUp(state);
    return 0;
}

void elosTestElosAuthorizedProcessInitializeExtErrConfigGet(void **state) {
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

    TEST("elosAuthorizedProcessInitialize");
    SHOULD("%s", "authorized process filter not initialized since config attribute not found");

    MOCK_FUNC_AFTER_CALL(samconfConfigGet, 0);
    expect_value(__wrap_samconfConfigGet, root, &mockRoot);
    expect_string(__wrap_samconfConfigGet, path, ELOS_CONFIG_ROOT "authorizedProcesses");
    expect_any(__wrap_samconfConfigGet, result);
    will_set_parameter(__wrap_samconfConfigGet, result, &mockConfig);
    will_return(__wrap_samconfConfigGet, SAMCONF_CONFIG_ERROR);

    result = elosAuthorizedProcessInitialize(&testFilter, &mockRoot);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
