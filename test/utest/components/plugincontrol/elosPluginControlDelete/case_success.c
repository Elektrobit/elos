// SPDX-License-Identifier: MIT

#include "elosPluginControlDelete_utest.h"

int elosTestElosPluginControlDeleteSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *path = "/test/123";
    samconfConfig_t config = {0};
    safuResultE_t result;
    elosPluginId_t id = 42;

    elosPluginParam_t param = {
        .config = &config,
        .data = NULL,
        .id = id,
        .path = path,
    };

    result = elosPluginControlNew(&test->plugin, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosPluginControlDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPluginControlDeleteSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosPluginControlDelete");
    SHOULD("%s", "test correct behaviour");

    result = elosPluginControlDelete(&test->plugin);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(test->plugin, NULL);
}
