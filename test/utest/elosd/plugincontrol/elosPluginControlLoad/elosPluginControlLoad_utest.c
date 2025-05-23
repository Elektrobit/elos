// SPDX-License-Identifier: MIT
#include "elosPluginControlLoad_utest.h"

#include <cmocka.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>

#define _TEST_CONFIG \
    "{\
    \"root\": {\
        \"elos\": {\
            \"UseEnv\": false,\
            \"ClientInputs\": {\
                \"PluginSearchPath\": \"build/Debug/cmake/src/plugins/clients/dummy/client_dummy.so\",\
                \"Plugins\": {\
                    \"LocalTcpClient\": {\
                        \"File\": \"client_dummy.so\",\
                        \"Run\": \"always\",\
                        \"Config\": {\
                            \"Port\": 54321,\
                            \"Interface\": \"127.0.0.1\",\
                            \"EventBlacklist\": \".event.messageCode 2000 EQ\",\
                            \"authorizedProcesses\": [\
                                \".process.uid 0 EQ .process.gid 0 EQ .process.exec '/bin/elosc' STRCMP AND\",\
                                \".process.gid 200 EQ .process.exec '/bin/elosc' STRCMP AND\",\
                                \".process.pid 1 EQ\"\
                            ]\
                        }\
                    },\
                    \"PublicTcpClient\": {\
                        \"File\": \"client_dummy.so\",\
                        \"Run\": \"always\",\
                        \"Config\": {\
                            \"Port\": 5422,\
                            \"Interface\": \"0.0.0.0\",\
                            \"EventBlacklist\": \".event.messageCode 2000 EQ\",\
                            \"authorizedProcesses\": []\
                        }\
                    },\
                    \"Pluginwithtoolongname\": {\
                        \"File\": \"client_dummy.so\",\
                        \"Run\": \"always\",\
                        \"Config\": {\
                            \"Port\": 54321,\
                            \"Interface\": \"127.0.0.1\",\
                            \"EventBlacklist\": \".event.messageCode 2000 EQ\",\
                            \"authorizedProcesses\": [\
                                \".process.uid 0 EQ .process.gid 0 EQ .process.exec '/bin/elosc' STRCMP AND\",\
                                \".process.gid 200 EQ .process.exec '/bin/elosc' STRCMP AND\",\
                                \".process.pid 1 EQ\"\
                            ]\
                        }\
                    }\
                }\
            }\
        }\
    }\
}"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosPluginControlLoadErrParam),
        TEST_CASE(elosTestElosPluginControlLoadSuccessLocal),
        TEST_CASE(elosTestElosPluginControlLoadSuccessName),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
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

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
