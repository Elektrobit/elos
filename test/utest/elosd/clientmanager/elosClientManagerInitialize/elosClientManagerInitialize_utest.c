// SPDX-License-Identifier: MIT
#include "elosClientManagerInitialize_utest.h"

#define _TEST_CONFIG \
    "{\
    \"root\": {\
        \"elos\": {\
            \"UseEnv\": false,\
            \"ClientInputs\": {\
                \"PluginSearchPath\": \"/usr/local/lib/elos/client\",\
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
                    }\
                }\
            }\
        }\
    }\
}"

TEST_SUITE_FUNC_PROTOTYPES(elosClientManagerInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosClientManagerInitializeSuccess),
                                       TEST_CASE(elosTestElosClientManagerInitializeSuccessDefaultSearchPath),
                                       TEST_CASE(elosTestElosClientManagerInitializeSuccessClientConfigNull),
                                       TEST_CASE(elosTestElosClientManagerInitializeErrClientManagerNull),
                                       TEST_CASE(elosTestElosClientManagerInitializeErrInitParamNull),
                                       TEST_CASE(elosTestElosClientManagerInitializeErrParamConfigNull),
                                       TEST_CASE(elosTestElosClientManagerInitializeErrParamPluginManagerNull),
                                       TEST_CASE(elosTestElosClientManagerInitializeExtErrPluginVectorCreate)};
    return RUN_TEST_SUITE(tests, elosClientManagerInitializeUtest);
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

static int elosClientManagerInitializeUtestSetup(UNUSED void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosClientManagerInitializeUtestTeardown(UNUSED void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
