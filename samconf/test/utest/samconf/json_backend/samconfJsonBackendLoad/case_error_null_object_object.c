// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>
#include <samconf/config_backend.h>
#include <stdio.h>

#include "mock_samconf.h"
#include "samconfJsonBackendLoad_utest.h"

int samconfTestSamconfJsonBackendLoadErrorNullObjectObjectSetup(void **state) {
    json_object *root = json_object_new_object();
    json_object_object_add(root, "test", NULL);
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

int samconfTestSamconfJsonBackendLoadErrorNullObjectObjectTeardown(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    json_object_put(data->backend->backendHandle);
    free(data->backend);
    samconfConfigDelete(data->config);
    free(data);
    return 0;
}

void samconfTestSamconfJsonBackendLoadErrorNullObjectObject(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    samconfConfigStatusE_t result;
    samconfConfig_t *config;

    TEST("samconfJsonBackendLoad");
    SHOULD("%s", "returns parse error since json_object type object contained a null value object");

    result = samconfJsonBackendLoad(data->backend, false, &config);

    data->config = config;

    assert_int_equal(result, SAMCONF_CONFIG_PARSE_ERROR);
}
