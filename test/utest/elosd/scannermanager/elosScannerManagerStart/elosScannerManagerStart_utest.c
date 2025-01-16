// SPDX-License-Identifier: MIT
#include "elosScannerManagerStart_utest.h"

#include <cmocka.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>

TEST_SUITE_FUNC_PROTOTYPES(elosScannerManagerStartUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosScannerManagerStartSuccess),
                                       TEST_CASE(elosTestElosScannerManagerStartSuccessEmptyPluginsList),
                                       TEST_CASE(elosTestElosScannerManagerStartSuccessNoPluginsList),
                                       TEST_CASE(elosTestElosScannerManagerStartErrScannerManagerNull),
                                       TEST_CASE(elosTestElosScannerManagerStartExtErrPluginManagerLoad)};
    return RUN_TEST_SUITE(tests, elosScannerManagerStartUtest);
}

samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config, const char *json) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    json_object *jobj = NULL;

    jobj = json_tokener_parse(json);

    result = elosUtilCreateMockConfig(jobj, false, config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    json_object_put(jobj);

    return result;
}

int elosMockConfigCleanup(samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;

    result = samconfConfigDelete(config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    return 0;
}

static int elosScannerManagerStartUtestSetup(UNUSED void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosScannerManagerStartUtestTeardown(UNUSED void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
