// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>

#include "elos/eventfilter/vector.h"
#include "elos/storagemanager/StorageBackend.h"
#include "elosPluginFilterLoaderLoad_utest.h"

int elosTestElosPluginFilterLoaderLoadSuccessSetup(UNUSED void **state) {
    samconfConfig_t *config = elosCreateConfigNode(NULL, "TestPlugin");
    samconfConfig_t *filterConfig = elosCreateConfigNode(config, "Filter");
    assert_string_equal(filterConfig->key, "Filter");

    elosCreateConfigString(filterConfig, NULL, ".event.messageCode 1 EQ");
    elosCreateConfigString(filterConfig, NULL, "skip_me_I_am_invalid");
    elosCreateConfigString(filterConfig, NULL, "1 1 EQ");
    *state = config;
    return 0;
}

int elosTestElosPluginFilterLoaderLoadSuccessTeardown(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    return 0;
}

void elosTestElosPluginFilterLoaderLoadSuccess(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    TEST("elosPluginFilterLoaderLoad");
    SHOULD("test if filter from a config section are loaded properly");

    elosStorageBackend_t backend = {0};
    elosPluginControl_t plugin = {.context = {.config = config, .data = &backend}};

    safuResultE_t result = elosPluginFilterLoaderLoad(&plugin);
    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(backend.filter.elementCount, 2);
    elosEventFilterVectorDeleteMembers(&backend.filter);
}
