// SPDX-License-Identifier: MIT

#include "elosPluginControlInitialize_utest.h"

int elosTestElosPluginControlInitializeErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    elosPluginControlParam_t const param = {0};
    result = elosPluginControlInitialize(&test->plugin, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosPluginControlInitializeErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosPluginControlDeleteMembers(&test->plugin);
    return 0;
}

void elosTestElosPluginControlInitializeErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosPluginControlParam_t const param = {0};
    elosPluginControlParam_t const invalidParamA = {.path = "123", .file = "456"};
    elosPluginControlParam_t const invalidParamB = {.path = "123"};
    elosPluginControl_t zeroedPlugin = {0};

    safuResultE_t result;

    TEST("elosPluginControlInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosPluginControlInitialize(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, param");
    result = elosPluginControlInitialize(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "zeroedPlugin, NULL");
    result = elosPluginControlInitialize(&zeroedPlugin, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "plugin, NULL");
    result = elosPluginControlInitialize(&test->plugin, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "plugin, invalidParamA");
    result = elosPluginControlInitialize(&test->plugin, &invalidParamA);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "plugin, invalidParamB");
    result = elosPluginControlInitialize(&test->plugin, &invalidParamB);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
