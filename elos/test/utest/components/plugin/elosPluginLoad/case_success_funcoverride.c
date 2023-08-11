// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosPluginLoad_utest.h"

int elosTestElosPluginLoadSuccessFuncOverrideSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    elosPluginParam_t const param = {
        .data = &test->data,
        .funcOverride.load = elosPluginFuncCustomName[ELOS_PLUGIN_FUNC_LOAD],
        .funcOverride.start = elosPluginFuncCustomName[ELOS_PLUGIN_FUNC_START],
        .funcOverride.stop = elosPluginFuncCustomName[ELOS_PLUGIN_FUNC_STOP],
        .funcOverride.unload = elosPluginFuncCustomName[ELOS_PLUGIN_FUNC_UNLOAD],
    };

    result = elosPluginInitialize(&test->plugin, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosPluginLoadSuccessFuncOverrideTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    MOCK_FUNC_AFTER_CALL(pthread_join, 0);
    expect_any(__wrap_pthread_join, __th);
    expect_any(__wrap_pthread_join, __thread_return);
    will_return(__wrap_pthread_join, 0);

    result = elosPluginDeleteMembers(&test->plugin);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_NEVER(pthread_join);
    MOCK_FUNC_NEVER(eventfd_read);

    return 0;
}

int _customPthreadCreate(UNUSED pthread_t *__restrict__ __newthread, UNUSED const pthread_attr_t *__restrict__ __attr,
                         UNUSED void *(*__start_routine)(void *), void *__restrict__ __arg) {
    elosPlugin_t *plugin = (elosPlugin_t *)__arg;

    // Convice the plugin we're loaded so we don't have to do start/stop here too
    plugin->state = PLUGIN_STATE_LOADED;

    return 0;
}

void elosTestElosPluginLoadSuccessFuncOverride(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    uint32_t expectedFuncBits = _CUSTOM_LOAD_BIT | _CUSTOM_START_BIT | _CUSTOM_STOP_BIT | _CUSTOM_UNLOAD_BIT;
    safuResultE_t result;

    TEST("elosPluginLoad");
    SHOULD("%s", "test correct behaviour of elosPluginLoad with function overloading");

    MOCK_FUNC_ALWAYS(eventfd_read);
    expect_any_always(__wrap_eventfd_read, __fd);
    expect_any_always(__wrap_eventfd_read, __value);
    will_return_always(__wrap_eventfd_read, 0);

    MOCK_FUNC_ALWAYS_WITH(pthread_create, _customPthreadCreate, NULL);

    result = elosPluginLoad(&test->plugin);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_NEVER(pthread_create);

    for (int i = 0; i < ELOS_PLUGIN_FUNC_COUNT; i += 1) {
        elosPluginFuncEntry_t *func = &test->plugin.func[i];

        assert_ptr_not_equal(func->ptr, NULL);
        assert_ptr_not_equal(func->name, NULL);
        assert_string_equal(func->name, elosPluginFuncCustomName[i]);

        result = func->ptr((void *)&test->plugin);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    assert_int_equal(test->data.customFuncBits, expectedFuncBits);
}
