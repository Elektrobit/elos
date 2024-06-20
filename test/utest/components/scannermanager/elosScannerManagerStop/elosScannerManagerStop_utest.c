// SPDX-License-Identifier: MIT
#include "elosScannerManagerStop_utest.h"

#include <cmocka.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>

#define _TEST_CONFIG \
    "{\
    \"root\": {\
        \"elos\": {\
            \"UseEnv\": false,\
            \"Scanner\": {\
                \"PluginSearchPath\": \"/usr/local/lib/elos/scanner\",\
                \"Plugins\": {\
                    \"ScannerDummy\": {\
                        \"File\": \"scanner_dummy.so\",\
                        \"Run\": \"always\"\
                    },\
                    \"DummyScanner\": {\
                        \"File\": \"scanner_dummy.so\",\
                        \"Run\": \"always\"\
                    }\
                }\
            }\
        }\
    }\
}"

TEST_SUITE_FUNC_PROTOTYPES(elosScannerManagerStopUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosScannerManagerStopSuccess),
                                       TEST_CASE(elosTestElosScannerManagerStopErrScannerManagerNull),
                                       TEST_CASE(elosTestElosScannerManagerStopErrParamPluginManagerNull),
                                       TEST_CASE(elosTestElosScannerManagerStopExtErrPluginManagerUnload)};
    return RUN_TEST_SUITE(tests, elosScannerManagerStopUtest);
}

samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    json_object *jobj = NULL;

    jobj = json_tokener_parse(_TEST_CONFIG);

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

static int elosScannerManagerStopUtestSetup(UNUSED void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosScannerManagerStopUtestTeardown(UNUSED void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
