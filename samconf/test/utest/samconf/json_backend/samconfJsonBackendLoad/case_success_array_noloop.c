// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>
#include <samconf/config_backend.h>
#include <stdio.h>

#include "mock_samconf.h"
#include "samconfJsonBackendLoad_utest.h"

int samconfTestSamconfJsonBackendLoadSuccessArrayNoloopSetup(void **state) {
    json_object *root = json_object_new_array();
    samconfConfigBackend_t *backend = malloc(sizeof(samconfConfigBackend_t));
    if (backend == NULL) {
        return 1;
    }
    samconfJsonBackendLoadTestData_t *data = malloc(sizeof(samconfJsonBackendLoadTestData_t));
    if (data == NULL) {
        free(backend);
        return 1;
    }
    data->backend = backend;
    backend->backendHandle = root;
    *state = data;
    return 0;
}

int samconfTestSamconfJsonBackendLoadSuccessArrayNoloopTeardown(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    json_object_put(data->backend->backendHandle);
    free(data->backend);
    samconfConfigDelete(data->config);
    free(data);
    return 0;
}

void samconfTestSamconfJsonBackendLoadSuccessArrayNoloop(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    samconfConfigStatusE_t result;
    samconfConfig_t *config;

    TEST("samconfJsonBackendLoad");
    SHOULD("%s", "returns no error when given a empty array.");

    result = samconfJsonBackendLoad(data->backend, false, &config);

    data->config = config;

    assert_int_equal(result, SAMCONF_CONFIG_OK);
}
