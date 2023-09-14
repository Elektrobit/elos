// SPDX-License-Identifier: MIT

#include <math.h>

#include "elosEventQueueRead_utest.h"
#define _TEST_PARAM_COUNT 3

int elosTestElosEventQueueReadErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventQueueReadErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventQueueReadErrParam(UNUSED void **state) {
    elosSession_t invalidSession = {0};
    elosSession_t validSession = {.fd = _MOCK_FD, .connected = true};
    elosEventVector_t *validEventVector = NULL;
    elosEventQueueId_t validId = 42;
    safuResultE_t result;

    TEST("elosEventQueueRead (parameter error)");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    uint32_t const combinations = ((uint32_t)pow(2, _TEST_PARAM_COUNT)) - 1;

    for (uint32_t i = 0; i < combinations; i += 1) {
        elosSession_t *session = (i & (1 << 0)) ? &validSession : NULL;
        elosEventQueueId_t eventQueueId = (i & (1 << 1)) ? validId : ELOS_ID_INVALID;
        elosEventVector_t **eventVector = (i & (1 << 2)) ? &validEventVector : NULL;

        SHOULD("test correct behaviour with combination i:%u", i);
        result = elosEventQueueRead(session, eventQueueId, eventVector);
        assert_true(result == SAFU_RESULT_FAILED);
    }

    PARAM("%s", "test correct behaviour with invalid session");
    result = elosEventQueueRead(&invalidSession, validId, &validEventVector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
