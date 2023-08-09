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

    elosPluginParam_t const param = {
        .config = &config,
        .data = &data,
        .id = id,
        .path = path,
    };

    result = elosPluginInitialize(&test->plugin, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_equal(test->plugin.config, &config);
    assert_ptr_equal(test->plugin.data, &data);
    assert_int_equal(test->plugin.id, id);
    assert_string_equal(test->plugin.path, path);
    assert_int_equal(test->plugin.state, PLUGIN_STATE_INITIALIZED);
    for (int i = 0; i < ELOS_PLUGIN_FUNC_COUNT; i += 1) {
        assert_ptr_not_equal(test->plugin.func[i].name, NULL);
        assert_string_equal(test->plugin.func[i].name, _funcDefaultName[i]);
        assert_ptr_equal(test->plugin.func[i].ptr, NULL);
    }
}
