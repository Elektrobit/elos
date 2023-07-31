// SPDX-License-Identifier: MIT

#include "elosEventDeserialize_utest.h"

int elosTestElosEventDeserializeSuccessSourceOnlySetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeserializeSuccessSourceOnlyTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeserializeSuccessSourceOnly(UNUSED void **state) {
    safuResultE_t retval;
    elosEvent_t resultEvent = {0};
    const char *testData = "{\"source\":{\"pid\":234,\"appName\":\"app234\",\"fileName\":\"/usr/local/app234\"}}";
    elosEventSource_t expectedSource = {.pid = 234, .appName = "app234", .fileName = "/usr/local/app234"};

    TEST("elosEventDeserialize");
    SHOULD("%s", "deserialize json string with only source to null");

    retval = elosEventDeserialize(&resultEvent, testData);

    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(resultEvent.source.pid, expectedSource.pid);
    assert_string_equal(resultEvent.source.appName, expectedSource.appName);
    assert_string_equal(resultEvent.source.fileName, expectedSource.fileName);

    elosEventDeleteMembers(&resultEvent);
}
