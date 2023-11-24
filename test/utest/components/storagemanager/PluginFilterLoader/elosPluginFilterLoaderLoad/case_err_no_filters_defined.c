// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>

#include "elos/eventfilter/vector.h"
#include "elos/storagemanager/StorageBackend.h"
#include "elosPluginFilterLoaderLoad_utest.h"

int elosTestElosPluginFilterLoaderLoadErrNoFiltersDefinedSetup(UNUSED void **state) {
    samconfConfig_t *config = elosCreateConfigNode(NULL, "TestPlugin");
    samconfConfig_t *filterConfig = elosCreateConfigNode(config, "Filter");
    assert_string_equal(filterConfig->key, "Filter");

    *state = config;
    return 0;
}

int elosTestElosPluginFilterLoaderLoadErrNoFiltersDefinedTeardown(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    return 0;
}

void elosTestElosPluginFilterLoaderLoadErrNoFiltersDefined(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    TEST("elosPluginFilterLoaderLoad");
    SHOULD("%s", "test if \"Filter\" section contains no filters, backend is setup properly");

    elosStorageBackend_t backend = {0};
    elosPluginControl_t plugin = {.context = {.config = config, .data = &backend}};

    safuResultE_t result = elosPluginFilterLoaderLoad(&plugin);
    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(backend.filter.elementCount, 0);
    elosEventFilterVectorDeleteMembers(&backend.filter);
}
