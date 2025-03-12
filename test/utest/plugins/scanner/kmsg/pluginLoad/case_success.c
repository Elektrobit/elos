// SPDX-License-Identifier: MIT

#include <errno.h>
#include <mock_libelosplugin.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>
#include <sys/stat.h>

#include "mock_kmsg_mapper.h"
#include "pluginLoad_utest.h"

int elosTestPluginLoadSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestPluginLoadSuccessTeardown(UNUSED void **state) {
    MOCK_FUNC_NEVER(open);
    MOCK_FUNC_NEVER(safuGetEnvOr);
    MOCK_FUNC_NEVER(strdup);
    MOCK_FUNC_NEVER(stat);
    return 0;
}

int _customStat(UNUSED const char *pathname, struct stat *statbuf) {
    statbuf->st_mode = S_IFCHR;
    return SAFU_RESULT_OK;
}

void elosTestPluginLoadSuccess(UNUSED void **state) {
    TEST("pluginLoad");
    SHOULD("load the plugin successfully");

    samconfConfig_t pluginConfig = {
        .key = "kmsg",
    };
    struct elosKmsgContext context = {0};

    elosPlugin_t plugin = {
        .config = &pluginConfig, .data = (void *)&context, .createPublisher = elosPluginControlCreatePublisher};

    const char *stateFile = ELOS_KMSGSTATEFILE_MOCK;
    const char *kmsgFile = ELOS_KMSG_FILE_MOCK;

    MOCK_FUNC_AFTER_CALL(calloc, 0);
    expect_value(__wrap_calloc, nmemb, 1);
    expect_any(__wrap_calloc, size);
    will_return(__wrap_calloc, &context);

    MOCK_FUNC_AFTER_CALL(elosKmsgMapperInit, 0);
    expect_any(elosKmsgMapperInit, mapper);
    will_return(elosKmsgMapperInit, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(eventfd, 0);
    expect_value(__wrap_eventfd, __count, 0);
    expect_value(__wrap_eventfd, __flags, 0);
    will_return(__wrap_eventfd, 0);

    MOCK_FUNC_ALWAYS(safuGetEnvOr);
    expect_string(__wrap_safuGetEnvOr, key, ELOS_KMSGSTATEFILE_KEY_MOCK);
    expect_value(__wrap_safuGetEnvOr, defaultValue, NULL);
    will_return(__wrap_safuGetEnvOr, ELOS_KMSGSTATEFILE_MOCK);

    MOCK_FUNC_ALWAYS(strdup);
    expect_string(__wrap_strdup, string, ELOS_KMSGSTATEFILE_MOCK);
    will_return(__wrap_strdup, stateFile);

    expect_string(__wrap_safuGetEnvOr, key, ELOS_KMSG_FILE_KEY_MOCK);
    expect_any(__wrap_safuGetEnvOr, defaultValue);
    will_return(__wrap_safuGetEnvOr, ELOS_KMSG_FILE_MOCK);

    expect_string(__wrap_strdup, string, ELOS_KMSG_FILE_MOCK);
    will_return(__wrap_strdup, kmsgFile);

    MOCK_FUNC_ALWAYS_WITH(stat, _customStat, NULL);

    MOCK_FUNC_ALWAYS(open);
    expect_value(__wrap_open, file, ELOS_KMSG_FILE_MOCK);
    expect_any(__wrap_open, flags);
    will_return(__wrap_open, 0);

    MOCK_FUNC_AFTER_CALL(access, 0);
    expect_value(__wrap_access, pathname, ELOS_KMSGSTATEFILE_MOCK);
    expect_any(__wrap_access, mode);
    will_return(__wrap_access, 1);

    MOCK_FUNC_WRAP(access_errno) = ENOENT;

    expect_value(__wrap_open, file, ELOS_KMSGSTATEFILE_MOCK);
    expect_any(__wrap_open, flags);
    expect_any(__wrap_open, mode);
    will_return(__wrap_open, 0);

    MOCK_FUNC_AFTER_CALL(ftruncate, 0);
    expect_any(__wrap_ftruncate, fd);
    expect_value(__wrap_ftruncate, length, 4096);
    will_return(__wrap_ftruncate, 0);

    char *mappedStateFile[4096];
    MOCK_FUNC_AFTER_CALL(mmap, 0);
    expect_any(__wrap_mmap, addr);
    expect_any(__wrap_mmap, length);
    expect_any(__wrap_mmap, prot);
    expect_any(__wrap_mmap, flags);
    expect_any(__wrap_mmap, fd);
    expect_any(__wrap_mmap, offset);
    will_return(__wrap_mmap, &mappedStateFile);

    MOCK_FUNC_AFTER_CALL(elosPluginControlCreatePublisher, 0);
    expect_any(elosPluginControlCreatePublisher, pluginControl);
    expect_any(elosPluginControlCreatePublisher, publisher);
    will_set_parameter(elosPluginControlCreatePublisher, publisher, NULL);
    will_return(elosPluginControlCreatePublisher, SAFU_RESULT_OK);

    safuResultE_t result = elosPluginConfig.load(&plugin);
    assert_int_equal(result, SAFU_RESULT_OK);
}
