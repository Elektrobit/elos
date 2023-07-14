// SPDX-License-Identifier: MIT
#include "elosEventDeepCopy_utest.h"
#include "mock_event_source.h"

int elosTestElosEventDeepCopyEventSourceDeepCopyErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeepCopyEventSourceDeepCopyErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeepCopyEventSourceDeepCopyError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEventSource_t testEventSource = {0};
    const elosEvent_t testSource = {.source = testEventSource};
    elosEvent_t testDest = {0};

    TEST("elosEventDeepCopy");
    SHOULD("%s", "return error since EventSourceDeepCopy returns error");

    MOCK_FUNC_AFTER_CALL(elosEventSourceDeepCopy, 0);
    expect_any(elosEventSourceDeepCopy, to);
    expect_any(elosEventSourceDeepCopy, from);
    will_return(elosEventSourceDeepCopy, SAFU_RESULT_FAILED);

    result = elosEventDeepCopy(&testDest, &testSource);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    elosEventSourceDeleteMembers(&testDest.source);
}
