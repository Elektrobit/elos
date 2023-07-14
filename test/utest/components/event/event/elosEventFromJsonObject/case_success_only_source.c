// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"

int elosTestElosEventFromJsonObjectSuccessSourceOnlySetup(void **state) {
    static elosTestState_t test = {0};
    safuResultE_t retval = elosEventNew(&test.result);
    assert_int_equal(retval, SAFU_RESULT_OK);
    *state = &test;
    return 0;
}

int elosTestElosEventFromJsonObjectSuccessSourceOnlyTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    json_object_put(test->object);
    elosEventDelete(test->result);
    return 0;
}

void elosTestElosEventFromJsonObjectSuccessSourceOnly(void **state) {
    safuResultE_t retval = SAFU_RESULT_FAILED;
    elosTestState_t *test = *(elosTestState_t **)state;
    const char *testData = "{\"Source\":{\"pid\":234,\"appName\":\"app234\",\"fileName\":\"/usr/local/app234\"}}";
    const elosEventSource_t expectedSource = {.pid = 234, .appName = "app234", .fileName = "/usr/local/app234"};

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "convert json object to event when source is null");

    test->object = json_tokener_parse(testData);
    assert_non_null(test->object);

    retval = elosEventFromJsonObject(test->result, test->object);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(test->result->source.pid, expectedSource.pid);
    assert_string_equal(test->result->source.appName, expectedSource.appName);
    assert_string_equal(test->result->source.fileName, expectedSource.fileName);
}
