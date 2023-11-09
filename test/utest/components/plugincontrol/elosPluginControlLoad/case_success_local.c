// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosPluginControlLoad_utest.h"

int elosTestElosPluginControlLoadSuccessLocalSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    samconfConfig_t config = {0};
    safuResultE_t result;
    elosPluginControlParam_t param = {
        .pluginType = PLUGIN_TYPE_SCANNER,
        .config = &config,
        .data = &test->data,
    };

    result = elosPluginControlInitialize(&test->pluginControl, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosPluginControlLoadSuccessLocalTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    MOCK_FUNC_AFTER_CALL(pthread_join, 0);
    expect_any(__wrap_pthread_join, __th);
    expect_any(__wrap_pthread_join, __thread_return);
    will_return(__wrap_pthread_join, 0);

    result = elosPluginControlDeleteMembers(&test->pluginControl);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_NEVER(pthread_join);
    MOCK_FUNC_NEVER(eventfd_read);

    return 0;
}

int _customPthreadCreate(UNUSED pthread_t *__restrict__ __newthread, UNUSED const pthread_attr_t *__restrict__ __attr,
                         UNUSED void *(*__start_routine)(void *), void *__restrict__ __arg) {
    elosPluginControl_t *pluginControl = (elosPluginControl_t *)__arg;
    elosPluginConfig_t const *pluginConfig = pluginControl->pluginConfig;
    safuResultE_t result;

    // Convice the plugin we're loaded so we don't have to do start/stop here too
    pluginControl->context.state = PLUGIN_STATE_LOADED;

    // Also simulate the plugin function calls used during the thread creation
    result = pluginConfig->load(&pluginControl->context);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = pluginConfig->start(&pluginControl->context);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

void elosTestElosPluginControlLoadSuccessLocal(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosPluginControlLoad");
    SHOULD("%s", "test correct behaviour of elosPluginControlLoad when loading from self (filename=NULL)");

    MOCK_FUNC_ALWAYS(eventfd_read);
    expect_any_always(__wrap_eventfd_read, __fd);
    expect_any_always(__wrap_eventfd_read, __value);
    will_return_always(__wrap_eventfd_read, 0);

    MOCK_FUNC_ALWAYS_WITH(pthread_create, _customPthreadCreate, NULL);

    result = elosPluginControlLoad(&test->pluginControl);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(test->data.customFuncBits, _CUSTOM_LOAD_BIT | _CUSTOM_START_BIT);
    assert_ptr_equal(test->pluginControl.pluginConfig->load, elosCustomLoad);
    assert_ptr_equal(test->pluginControl.pluginConfig->unload, elosCustomUnload);
    assert_ptr_equal(test->pluginControl.pluginConfig->start, elosCustomStart);
    assert_ptr_equal(test->pluginControl.pluginConfig->stop, elosCustomStop);
    assert_ptr_equal(test->pluginControl.pluginConfig->type, PLUGIN_TYPE_SCANNER);
}
