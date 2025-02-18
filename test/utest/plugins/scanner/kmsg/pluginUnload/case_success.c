// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>
#include <string.h>

#include "pluginUnload_utest.h"

int elosTestPluginUnloadSuccessSetup(UNUSED void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestPluginUnloadSuccessTeardown(UNUSED void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    MOCK_FUNC_NEVER(close);
    MOCK_FUNC_NEVER(free);
    MOCK_FUNC_NEVER(elosPluginControlDeletePublisher);
    free(test->context.kmsgFile);
    return 0;
}

void elosTestPluginUnloadSuccess(UNUSED void **state) {
    TEST("pluginUnload");
    SHOULD("unload the plugin successfully");

    elosTestState_t *test = *(elosTestState_t **)state;

    samconfConfig_t pluginConfig = {
        .key = "kmsg",
    };

    elosPlugin_t plugin = {.config = &pluginConfig};
    plugin.deletePublisher = elosPluginControlDeletePublisher;

    PARAM("context is NULL");

    plugin.data = NULL;
    safuResultE_t result = elosPluginConfig.unload(&plugin);

    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("context->kmsgFile is NULL");

    test->context.kmsgFile = NULL;
    plugin.data = (void *)&(test->context);
    test->context.kmsgPollFd = MOCK_FD;
    test->context.cmdPollFd = MOCK_FD + 1;

    MOCK_FUNC_ALWAYS(close);
    expect_value(__wrap_close, fd, MOCK_FD);
    will_return(__wrap_close, 0);
    expect_value(__wrap_close, fd, MOCK_FD + 1);
    will_return(__wrap_close, 0);

    MOCK_FUNC_ALWAYS(elosPluginControlDeletePublisher);
    expect_any(elosPluginControlDeletePublisher, pluginControl);
    expect_any(elosPluginControlDeletePublisher, publisher);
    will_return(elosPluginControlDeletePublisher, SAFU_RESULT_OK);

    MOCK_FUNC_ALWAYS(free);
    expect_value(__wrap_free, ptr, plugin.data);

    result = elosPluginConfig.unload(&plugin);

    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("context is fully allocated");

    test->context.kmsgFile = strdup("dummy");
    test->context.flags = SCANNER_KMSG_FILE_CREATED;
    test->context.kmsgPollFd = MOCK_FD;
    test->context.cmdPollFd = MOCK_FD + 1;
    plugin.data = (void *)&(test->context);

    expect_value(__wrap_close, fd, MOCK_FD);
    will_return(__wrap_close, 0);
    expect_value(__wrap_close, fd, MOCK_FD + 1);
    will_return(__wrap_close, 0);

    MOCK_FUNC_AFTER_CALL(unlink, 0);
    expect_value(__wrap_unlink, pathname, test->context.kmsgFile);
    will_return(__wrap_unlink, 0);

    expect_value(__wrap_free, ptr, test->context.kmsgFile);

    expect_any(elosPluginControlDeletePublisher, pluginControl);
    expect_any(elosPluginControlDeletePublisher, publisher);
    will_return(elosPluginControlDeletePublisher, SAFU_RESULT_OK);

    expect_value(__wrap_free, ptr, plugin.data);

    result = elosPluginConfig.unload(&plugin);

    assert_int_equal(result, SAFU_RESULT_OK);
}
