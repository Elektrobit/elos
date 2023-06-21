// SPDX-License-Identifier: MIT

#include "elosEventDeserialize_utest.h"

int elosTestElosEventDeserializeSuccessNullEventSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeserializeSuccessNullEventTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeserializeSuccessNullEvent(UNUSED void **state) {
    safuResultE_t retval;
    elosEvent_t resultEvent = {0};
    const elosEvent_t expectedEvent = {0};

    TEST("elosEventDeserialize");
    SHOULD("%s", "deserialize json string to event when string is empty json string");

    retval = elosEventDeserialize(&resultEvent, "{}");

    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(resultEvent.date.tv_sec, expectedEvent.date.tv_sec);
    assert_int_equal(resultEvent.date.tv_nsec, expectedEvent.date.tv_nsec);
    assert_null(resultEvent.source.appName);
    assert_null(resultEvent.source.fileName);
    assert_int_equal(resultEvent.source.pid, 0);
    assert_int_equal(resultEvent.severity, expectedEvent.severity);
    assert_int_equal(resultEvent.classification, expectedEvent.classification);
    assert_ptr_equal(resultEvent.hardwareid, expectedEvent.hardwareid);
    assert_int_equal(resultEvent.messageCode, expectedEvent.messageCode);
    assert_ptr_equal(resultEvent.payload, expectedEvent.payload);

    elosEventDeleteMembers(&resultEvent);
}
