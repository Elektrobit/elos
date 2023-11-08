// SPDX-License-Identifier: MIT

#include "elosPluginInitialize_utest.h"

int elosTestElosPluginInitializeSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginInitializeSuccessTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosPluginDeleteMembers(&test->plugin);

    return 0;
}

void elosTestElosPluginInitializeSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *path = "/test/123";
    samconfConfig_t config = {0};
    safuResultE_t result;
    elosPluginId_t id = 42;
    uint64_t data = 0;

    TEST("elosPluginInitialize");
    SHOULD("%s", "test correct behaviour of elosPluginInitialize");

    for (int i = 0; i < 2; i += 1) {
        elosPluginParam_t param = {
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

        result = elosPluginInitialize(&test->plugin, &param);
        assert_int_equal(result, SAFU_RESULT_OK);
        assert_int_equal(test->plugin.state, PLUGIN_STATE_INITIALIZED);
        assert_ptr_equal(test->plugin.config, param.config);
        assert_ptr_equal(test->plugin.data, param.data);
        assert_int_equal(test->plugin.id, param.id);
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

        elosPluginDeleteMembers(&test->plugin);
    }
}
