// SPDX-License-Identifier: MIT

#include "elosPluginInitialize_utest.h"

int elosTestElosPluginInitializeErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    elosPluginParam_t const param = {0};
    result = elosPluginInitialize(&test->plugin, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosPluginInitializeErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosPluginDeleteMembers(&test->plugin);
    return 0;
}

void elosTestElosPluginInitializeErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosPluginParam_t const param = {0};
    elosPluginParam_t const invalidParamA = {.path = "123", .file = "456"};
    elosPluginParam_t const invalidParamB = {.path = "123"};
    elosPlugin_t zeroedPlugin = {0};

    safuResultE_t result;

    TEST("elosPluginInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosPluginInitialize(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, param");
    result = elosPluginInitialize(NULL, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "zeroedPlugin, NULL");
    result = elosPluginInitialize(&zeroedPlugin, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "plugin, NULL");
    result = elosPluginInitialize(&test->plugin, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "plugin, invalidParamA");
    result = elosPluginInitialize(&test->plugin, &invalidParamA);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "plugin, invalidParamB");
    result = elosPluginInitialize(&test->plugin, &invalidParamB);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
