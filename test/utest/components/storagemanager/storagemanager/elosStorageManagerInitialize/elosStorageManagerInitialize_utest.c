// SPDX-License-Identifier: MIT
#include "elosStorageManagerInitialize_utest.h"

#define _TEST_CONFIG \
    "{\
    \"root\": {\
        \"elos\": {\
            \"EventLogging\": {\
            }\
        }\
    }\
}\
"

TEST_SUITE_FUNC_PROTOTYPES(elosStorageManagerInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosStorageManagerInitializeSuccess),
        TEST_CASE(elosTestElosStorageManagerInitializeSuccessDefaultSearchPath),
        TEST_CASE(elosTestElosStorageManagerInitializeSuccessBackendConfigNull),
        TEST_CASE(elosTestElosStorageManagerInitializeErrStorageManagerNull),
        TEST_CASE(elosTestElosStorageManagerInitializeErrInitParamNull),
        TEST_CASE(elosTestElosStorageManagerInitializeErrParamConfigNull),
        TEST_CASE(elosTestElosStorageManagerInitializeErrParamPluginManagerNull),
        TEST_CASE(elosTestElosStorageManagerInitializeExtErrPluginVectorCreate),
    };
    return RUN_TEST_SUITE(tests, elosStorageManagerInitializeUtest);
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

static int elosStorageManagerInitializeUtestSetup(UNUSED void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosStorageManagerInitializeUtestTeardown(UNUSED void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
