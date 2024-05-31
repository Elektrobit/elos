// SPDX-License-Identifier: MIT
#include "elosMessageHandlerSendJson_utest.h"

#include <string.h>

TEST_SUITE_FUNC_PROTOTYPES(elosMessageHandlerSendJson)

int main(UNUSED int argc, UNUSED char **argv) {
    const struct CMUnitTest unitTests[] = {TEST_CASE(elosTestElosMessageHandlerSendJsonSuccess),
                                           TEST_CASE(elosTestElosMessageHandlerSendJsonExterrJsonObject),
                                           TEST_CASE(elosTestElosMessageHandlerSendJsonExterrSend)};

    return RUN_TEST_SUITE(unitTests, elosMessageHandlerSendJson);
}

elosUnitTestState_t *elosMessageHandlerSendJsonCreateTestData(const char *objectStr) {
    elosUnitTestState_t *testData = test_malloc(sizeof(elosUnitTestState_t));
    assert_non_null(testData);

    char *string = (char *)test_malloc((strlen(objectStr) + 1) * sizeof(char));
    strcpy(string, objectStr);
    testData->jsonString = string;

    json_object *json = json_tokener_parse(objectStr);
    testData->jsonObject = json;

    return testData;
}

int elosMessageHandlerSendJsonFreeTestData(elosUnitTestState_t *data) {
    test_free(data->jsonString);
    json_object_put(data->jsonObject);
    test_free(data);
    return 0;
}

static int elosMessageHandlerSendJsonSetup(UNUSED void **state) {
    return 0;
}

static int elosMessageHandlerSendJsonTeardown(UNUSED void **state) {
    return 0;
}
