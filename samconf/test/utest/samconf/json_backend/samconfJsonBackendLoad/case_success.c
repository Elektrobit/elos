// SPDX-License-Identifier: MIT

#include <json-c/json.h>
#include <samconf/config_backend.h>
#include <stdio.h>

#include "samconfJsonBackendLoad_utest.h"

int samconfTestSamconfJsonBackendLoadSuccessSetup(void **state) {
    json_object *jobj = json_tokener_parse(JSON_CONFIG);
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

int samconfTestSamconfJsonBackendLoadSuccessTeardown(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    json_object_put(data->backend->backendHandle);
    free(data->backend);
    samconfConfigDelete(data->config);
    free(data);
    return 0;
}

void samconfTestSamconfJsonBackendLoadSuccess(void **state) {
    samconfJsonBackendLoadTestData_t *data = (samconfJsonBackendLoadTestData_t *)*state;
    samconfConfigStatusE_t result;
    samconfConfig_t *config;
    const samconfConfig_t *resultConfig;
    const samconfConfig_t *root;
    const samconfConfig_t *categoryA;
    const samconfConfig_t *categoryB;
    const char *resultString;
    int resultInt;
    bool resultBool;
    double resultDouble;

    TEST("samconfJsonBackendLoad");
    SHOULD("%s", "return a config_t object containing the date from the inputted json string.");

    result = samconfJsonBackendLoad(data->backend, false, &config);
    data->config = config;

    assert_int_equal(result, SAMCONF_CONFIG_OK);

    assert_string_equal(config->key, "config");
    assert_int_equal(config->type, SAMCONF_CONFIG_VALUE_OBJECT);
    assert_int_equal(config->childCount, 1);
    assert_null(config->parent);

    result = samconfConfigGet(config, "/root", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_OBJECT);
    assert_int_equal(resultConfig->childCount, 2);
    assert_ptr_equal(resultConfig->parent, config);
    root = resultConfig;

    result = samconfConfigGet(config, "/root/categoryA", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_OBJECT);
    assert_int_equal(resultConfig->childCount, 4);
    assert_ptr_equal(resultConfig->parent, root);
    categoryA = resultConfig;

    result = samconfConfigGet(config, "/root/categoryB", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_ARRAY);
    assert_int_equal(resultConfig->childCount, 4);
    assert_ptr_equal(resultConfig->parent, root);
    categoryB = resultConfig;

    result = samconfConfigGet(config, "/root/categoryA/value", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_STRING);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetString(config, "/root/categoryA/value", &resultString);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(resultString, "the value of a configuration option");
    assert_ptr_equal(resultConfig->parent, categoryA);

    result = samconfConfigGet(config, "/root/categoryA/number", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_INT);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetInt32(config, "/root/categoryA/number", &resultInt);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultInt, 23);
    assert_ptr_equal(resultConfig->parent, categoryA);

    result = samconfConfigGet(config, "/root/categoryA/bool", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_BOOLEAN);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetBool(config, "/root/categoryA/bool", &resultBool);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultBool, true);
    assert_ptr_equal(resultConfig->parent, categoryA);

    result = samconfConfigGet(config, "/root/categoryA/real", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_REAL);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetReal(config, "/root/categoryA/real", &resultDouble);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_double_equal(resultDouble, 4.231, 0.0);
    assert_ptr_equal(resultConfig->parent, categoryA);

    result = samconfConfigGet(config, "/root/categoryB/0", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_STRING);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetString(config, "/root/categoryB/0", &resultString);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(resultString, "itemA");
    assert_ptr_equal(resultConfig->parent, categoryB);

    result = samconfConfigGet(config, "/root/categoryB/1", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_STRING);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetString(config, "/root/categoryB/1", &resultString);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(resultString, "itemB");
    assert_ptr_equal(resultConfig->parent, categoryB);

    result = samconfConfigGet(config, "/root/categoryB/2", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_STRING);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetString(config, "/root/categoryB/2", &resultString);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_string_equal(resultString, "itemC");
    assert_ptr_equal(resultConfig->parent, categoryB);

    result = samconfConfigGet(config, "/root/categoryB/3", &resultConfig);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultConfig->type, SAMCONF_CONFIG_VALUE_INT);
    assert_int_equal(resultConfig->childCount, 0);
    result = samconfConfigGetInt32(config, "/root/categoryB/3", &resultInt);
    assert_int_equal(result, SAMCONF_CONFIG_OK);
    assert_int_equal(resultInt, 42);
    assert_ptr_equal(resultConfig->parent, categoryB);
}
