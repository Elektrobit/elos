// SPDX-License-Identifier: MIT
#include "elosStorageManagerDeleteMembers_utest.h"

#include <cmocka.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>

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

TEST_SUITE_FUNC_PROTOTYPES(elosStorageManagerDeleteMembersUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosStorageManagerDeleteMembersSuccess),
        TEST_CASE(elosTestElosStorageManagerDeleteMembersErrStorageManagerNull),
        TEST_CASE(elosTestElosStorageManagerDeleteMembersErrParamPluginManagerNull),
        TEST_CASE(elosTestElosStorageManagerDeleteMembersExtErrPluginVectorFree),
    };
    return RUN_TEST_SUITE(tests, elosStorageManagerDeleteMembersUtest);
}

samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    json_object *testJobj = NULL;

    testJobj = json_tokener_parse(_TEST_CONFIG);

    result = elosTestCreateMockConfig(testJobj, false, config);
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

static int elosStorageManagerDeleteMembersUtestSetup(UNUSED void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosStorageManagerDeleteMembersUtestTeardown(UNUSED void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
