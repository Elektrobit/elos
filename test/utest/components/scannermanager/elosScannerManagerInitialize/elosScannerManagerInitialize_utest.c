// SPDX-License-Identifier: MIT
#include "elosScannerManagerInitialize_utest.h"

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

TEST_SUITE_FUNC_PROTOTYPES(elosScannerManagerInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosScannerManagerInitializeSuccess)};
    return RUN_TEST_SUITE(tests, elosScannerManagerInitializeUtest);
}

samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    json_object *testJobj = NULL;

    testJobj = json_tokener_parse(_TEST_CONFIG);

    result = elosUtilCreateMockConfig(testJobj, false, config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    json_object_put(testJobj);

    return result;
}

int elosMockConfigCleanup(samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;

    result = samconfConfigDelete(config);
    assert_int_equal(result, SAMCONF_CONFIG_OK);

    return 0;
}

static int elosScannerManagerInitializeUtestSetup(UNUSED void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosScannerManagerInitializeUtestTeardown(UNUSED void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
