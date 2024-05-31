// SPDX-License-Identifier: MIT
#include "elosMessageEventSubscribe_utest.h"

#include "cmocka_mocks/mock_jsonc.h"

TEST_SUITE_FUNC_PROTOTYPES(elosMessageEventSubscribeUtest)

int elosAssertStringArraysEqual(const long unsigned int valueInt, const long unsigned int checkValueDataInt) {
    char **value = (char **)valueInt;
    elosArrayStringTestData_t *checkValueData = (elosArrayStringTestData_t *)checkValueDataInt;
    int i;
    for (i = 0; i < checkValueData->count; i++) {
        if (strcmp(value[i], checkValueData->strings[i]) != 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosMessageEventSubscribeCompleteFailure),
        TEST_CASE(elosTestElosMessageEventSubscribeFilterNodeNull),
        TEST_CASE(elosTestElosMessageEventSubscribeFilterAllocateFailed),
        TEST_CASE(elosTestElosMessageEventSubscribeNoFilter),
        TEST_CASE(elosTestElosMessageEventSubscribeNoJson),
        TEST_CASE(elosTestElosMessageEventSubscribeNoLoop),
        TEST_CASE(elosTestElosMessageEventSubscribeNoString),
        TEST_CASE(elosTestElosMessageEventSubscribeQueueIdNull),
        TEST_CASE(elosTestElosMessageEventSubscribeSuccess),
        TEST_CASE(elosTestElosMessageEventSubscribeStrdupFailed),
        TEST_CASE(elosTestElosMessageEventSubscribeResponseNewArrayFailed),
        TEST_CASE(elosTestElosMessageEventSubscribeResponseNewIntFailed),
        TEST_CASE(elosTestElosMessageEventSubscribeResponseSendFailed),
    };

    return RUN_TEST_SUITE(tests, elosMessageEventSubscribeUtest);
}

static int elosMessageEventSubscribeUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosMessageEventSubscribeUtestTeardown(UNUSED void **state) {
    return 0;
}

int elosCheckJsonObject(const long unsigned int jsonParam, const long unsigned int jsonExpected) {
    json_object *param = (json_object *)jsonParam;
    json_object *expected = (json_object *)jsonExpected;
    if (NULL == param) {
        print_error("Parameter is NULL\n");
        return 0;
    }
    if (NULL == expected) {
        print_error("Expected NULL but found %s\n", json_object_to_json_string(param));
        return 0;
    }
    if (!json_object_equal(param, expected)) {
        print_message("%s != %s\n", json_object_to_json_string(param), json_object_to_json_string(expected));
        return 0;
    }
    return 1;
}
