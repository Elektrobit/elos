// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerProcess_utest.h"
#include "mock_eventfilternodevector.h"

int elosTestEloEventFilterManagerProcessSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterManagerProcessSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterManagerProcessSuccess(UNUSED void **state) {
    elosEventFilterManager_t efm = {.flags = SAFU_FLAG_INITIALIZED_BIT};
    elosEvent_t const event = {0};
    safuResultE_t result;

    TEST("elosEventFilterManagerProcess");
    SHOULD("%s", "Test successful code execution");

    // Simulating the whole environment would be very time-consuming;
    // elosEventFilterManagerProcess simply passes the event down to elosEventFilterNodeVectorProcess,
    // with elosEventFilterNodeVectorProcess having its own comprehensive test suite,
    // we simply mock said function and check if the parameters were passed on properly
    MOCK_FUNC_AFTER_CALL(elosEventFilterNodeVectorProcess, 0);
    expect_value(elosEventFilterNodeVectorProcess, eventFilterNodeVector, &efm.eventFilterNodeVector);
    expect_value(elosEventFilterNodeVectorProcess, filterStack, NULL);
    expect_value(elosEventFilterNodeVectorProcess, event, &event);
    will_return(elosEventFilterNodeVectorProcess, SAFU_RESULT_OK);

    result = elosEventFilterManagerProcess(&efm, &event);
    assert_int_equal(result, SAFU_RESULT_OK);
}
