// SPDX-License-Identifier: MIT
#include "elosStorageManagerStop_utest.h"

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

TEST_SUITE_FUNC_PROTOTYPES(elosStorageManagerStopUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosStorageManagerStopSuccess),
        TEST_CASE(elosTestElosStorageManagerStopErrStorageManagerNull),
        TEST_CASE(elosTestElosStorageManagerStopErrParamPluginManagerNull),
        TEST_CASE(elosTestElosStorageManagerStopExtErrPluginManagerUnload),
    };
    return RUN_TEST_SUITE(tests, elosStorageManagerStopUtest);
}

samconfConfigStatusE_t elosGetMockConfig(samconfConfig_t *config) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    json_object *jobj = NULL;

    jobj = json_tokener_parse(_TEST_CONFIG);

    result = elosTestCreateMockConfig(jobj, false, config);
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

static int elosStorageManagerStopUtestSetup(UNUSED void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int elosStorageManagerStopUtestTeardown(UNUSED void **state) {
    free(*state);
    *state = NULL;

    return 0;
}
