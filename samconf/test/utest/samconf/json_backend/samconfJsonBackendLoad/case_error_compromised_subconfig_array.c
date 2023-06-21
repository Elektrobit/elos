// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <json-c/json.h>
#include <samconf/config_backend.h>
#include <stdio.h>

#include "mock_samconf.h"
#include "samconfJsonBackendLoad_utest.h"

int samconfTestSamconfJsonBackendLoadErrorCompromisedSubconfigArraySetup(void **state) {
    json_object *jobj = json_tokener_parse(JSON_CONFIG_SWAPPED);
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
    backend->backendHandle = jobj;
    *state = data;
    return 0;
}

int samconfTestSamconfJsonBackendLoadErrorCompromisedSubconfigArrayTeardown(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    json_object_put(data->backend->backendHandle);
    free(data->backend);
    samconfConfigDelete(data->config);
    free(data);
    return 0;
}

void samconfTestSamconfJsonBackendLoadErrorCompromisedSubconfigArray(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    samconfConfigStatusE_t result;
    samconfConfig_t *config;

    TEST("samconfJsonBackendLoad");
    SHOULD("%s", "returns error code given by failing samconfConfigNew during subnode creation, type array.");

    MOCK_FUNC_AFTER_CALL(samconfConfigNew, 2);

    expect_any(samconfConfigNew, config);

    will_return(samconfConfigNew, 300);

    result = samconfJsonBackendLoad(data->backend, false, &config);

    data->config = config;

    assert_int_equal(result, 300);
}
