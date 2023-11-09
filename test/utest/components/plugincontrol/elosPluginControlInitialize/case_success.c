// SPDX-License-Identifier: MIT

#include "elosPluginControlInitialize_utest.h"

int elosTestElosPluginControlInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginControlInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosPluginControlDeleteMembers(&test->plugin);

    return 0;
}

void elosTestElosPluginControlInitializeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *path = "/test/123";
    samconfConfig_t config = {0};
    safuResultE_t result;
    elosPluginId_t id = 42;
    uint64_t data = 0;

    TEST("elosPluginControlInitialize");
    SHOULD("%s", "test correct behaviour of elosPluginControlInitialize");

    for (int i = 0; i < 2; i += 1) {
        elosPluginControlParam_t param = {
            .config = &config,
            .data = &data,
            .id = id,
            .path = path,
        };

        if (i == 0) {
            PARAM("Test initialization with .path and .samconf");
        } else {
            PARAM("Test initialization with .file");
            param.config = NULL;
            param.path = NULL;
            param.file = "/test/456";
        }

        result = elosPluginControlInitialize(&test->plugin, &param);
        assert_int_equal(result, SAFU_RESULT_OK);
        assert_int_equal(test->plugin.context.state, PLUGIN_STATE_INITIALIZED);
        assert_ptr_equal(test->plugin.context.config, param.config);
        assert_ptr_equal(test->plugin.context.data, param.data);
        assert_int_equal(test->plugin.context.id, param.id);
        for (int o = 0; o < ELOS_PLUGIN_FUNC_COUNT; o += 1) {
            assert_ptr_not_equal(test->plugin.func[o].name, NULL);
            assert_string_equal(test->plugin.func[o].name, elosPluginFuncDefaultName[o]);
            assert_ptr_equal(test->plugin.func[o].ptr, NULL);
        }

        if (i == 0) {
            assert_ptr_equal(test->plugin.file, NULL);
            assert_ptr_not_equal(test->plugin.path, NULL);
            assert_string_equal(test->plugin.path, param.path);
        } else {
            assert_ptr_equal(test->plugin.path, NULL);
            assert_ptr_not_equal(test->plugin.file, NULL);
            assert_string_equal(test->plugin.file, param.file);
        }

        elosPluginControlDeleteMembers(&test->plugin);
    }
}
