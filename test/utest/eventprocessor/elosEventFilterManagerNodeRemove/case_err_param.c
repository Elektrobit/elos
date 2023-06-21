// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerNodeRemove_utest.h"
#include "mock_eventfilternodevector.h"

int elosTestEloEventFilterManagerNodeRemoveErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestEloEventFilterManagerNodeRemoveErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterManagerNodeRemoveErrParam(UNUSED void **state) {
    elosEventFilterNodeId_t eventFilterNodeId = 12;
    elosEventFilterManager_t zeroedEventFilterManager = {0};
    safuResultE_t result;

    TEST("elosEventFilterManagerNodeRemove");
    SHOULD("%s", "fail as invalid parameter is given.");

    PARAM("%s", "NULL, eventFilterNodeId");
    result = elosEventFilterManagerNodeRemove(NULL, eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "zeroedEventFilterManager, eventFilterNodeId");
    result = elosEventFilterManagerNodeRemove(&zeroedEventFilterManager, eventFilterNodeId);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "zeroedEventFilterManager, ELOS_ID_INVALID");
    result = elosEventFilterManagerNodeRemove(&zeroedEventFilterManager, ELOS_ID_INVALID);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, ELOS_ID_INVALID");
    result = elosEventFilterManagerNodeRemove(NULL, ELOS_ID_INVALID);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
