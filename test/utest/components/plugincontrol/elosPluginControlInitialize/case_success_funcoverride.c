// SPDX-License-Identifier: MIT

#include "elosPluginControlInitialize_utest.h"

#define _LOAD_NAME_BIT   (1 << 0)
#define _START_NAME_BIT  (1 << 1)
#define _STOP_NAME_BIT   (1 << 2)
#define _UNLOAD_NAME_BIT (1 << 3)

int elosTestElosPluginControlInitializeSuccessFuncOverrideSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginControlInitializeSuccessFuncOverrideTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosPluginControlDeleteMembers(&test->plugin);

    return 0;
}

void elosTestElosPluginControlInitializeSuccessFuncOverride(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    char const *path = "/test/123";
    samconfConfig_t config = {0};
    safuResultE_t result;
    elosPluginId_t id = 42;
    uint64_t data = 0;

    size_t const numberOfTestCases = ELOS_PLUGIN_FUNC_COUNT * ELOS_PLUGIN_FUNC_COUNT;  // simple integer power of 2

    char const *funcOverrideName[ELOS_PLUGIN_FUNC_COUNT] = {
        [ELOS_PLUGIN_FUNC_LOAD] = "customLoad",
        [ELOS_PLUGIN_FUNC_START] = "customStart",
        [ELOS_PLUGIN_FUNC_STOP] = "customStop",
        [ELOS_PLUGIN_FUNC_UNLOAD] = "customUnload",
    };

    TEST("elosPluginControlInitialize");
    SHOULD("%s", "test correct behaviour of elosPluginControlInitialize with function overloading");

    // i==0 is handled in case_success.c, so we skip it here for easier test case construction
    for (size_t i = 1; i < numberOfTestCases; i += 1) {
        elosPluginParam_t const param = {
            .config = &config,
            .data = &data,
            .id = id,
            .path = path,
            .funcOverride =
                {
                    .load = (i & _LOAD_NAME_BIT) ? funcOverrideName[ELOS_PLUGIN_FUNC_LOAD] : NULL,
                    .start = (i & _START_NAME_BIT) ? funcOverrideName[ELOS_PLUGIN_FUNC_START] : NULL,
                    .stop = (i & _STOP_NAME_BIT) ? funcOverrideName[ELOS_PLUGIN_FUNC_STOP] : NULL,
                    .unload = (i & _UNLOAD_NAME_BIT) ? funcOverrideName[ELOS_PLUGIN_FUNC_UNLOAD] : NULL,
                },
        };

        result = elosPluginControlInitialize(&test->plugin, &param);
        assert_int_equal(result, SAFU_RESULT_OK);
        assert_ptr_equal(test->plugin.config, &config);
        assert_ptr_equal(test->plugin.data, &data);
        assert_int_equal(test->plugin.id, id);
        assert_ptr_not_equal(test->plugin.path, NULL);
        assert_string_equal(test->plugin.path, path);
        assert_int_equal(test->plugin.state, PLUGIN_STATE_INITIALIZED);

        for (size_t o = 0; o < ELOS_PLUGIN_FUNC_COUNT; o += 1) {
            elosPluginFuncEntry_t *func = test->plugin.func;
            char const *testFuncName = elosPluginFuncDefaultName[o];
            size_t const nameBit = (1 << o);

            if (i & nameBit) {
                testFuncName = funcOverrideName[o];
            }

            // printf("#### %ld, %ld, %ld\n", i, o, nameBit);
            assert_ptr_not_equal(func[o].name, NULL);
            assert_string_equal(func[o].name, testFuncName);
        }

        result = elosPluginControlDeleteMembers(&test->plugin);
        assert_int_equal(result, SAFU_RESULT_OK);
    }
}
