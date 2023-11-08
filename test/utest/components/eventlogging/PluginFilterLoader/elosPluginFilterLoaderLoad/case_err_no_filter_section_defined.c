// SPDX-License-Identifier: MIT

#include <samconf/samconf.h>

#include "elos/eventfilter/vector.h"
#include "elos/eventlogging/StorageBackend.h"
#include "elosPluginFilterLoaderLoad_utest.h"

int elosTestElosPluginFilterLoaderLoadErrNoFilterSectionDefinedSetup(UNUSED void **state) {
    samconfConfig_t *config = elosCreateConfigNode(NULL, "TestPlugin");

    *state = config;
    return 0;
}

int elosTestElosPluginFilterLoaderLoadErrNoFilterSectionDefinedTeardown(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    samconfConfigDelete(config);
    return 0;
}

void elosTestElosPluginFilterLoaderLoadErrNoFilterSectionDefined(UNUSED void **state) {
    samconfConfig_t *config = *(samconfConfig_t **)state;
    TEST("elosPluginFilterLoaderLoad");
    SHOULD("%s", "test if no \"Filter\" section is defined, backend is setup properly");

    elosStorageBackend_t backend = {0};
    elosPlugin_t plugin = {.context = {.config = config, .data = &backend}};

    safuResultE_t result = elosPluginFilterLoaderLoad(&plugin);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    assert_int_equal(backend.filter.elementCount, 0);
    elosEventFilterVectorDeleteMembers(&backend.filter);
}
