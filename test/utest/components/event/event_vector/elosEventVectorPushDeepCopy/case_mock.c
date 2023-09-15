// SPDX-License-Identifier: MIT
#include "elosEventVectorPushDeepCopy_utest.h"
#include "mock_event.h"
#include "safu/mock_vector.h"

static inline void _testMock(safuResultE_t res) {
    elosEvent_t event = {0};
    elosEventVector_t vector = {0};
    safuResultE_t retval;

    MOCK_FUNC_AFTER_CALL(elosEventDeepCopy, 0);
    expect_value(elosEventDeepCopy, from, &event);
    expect_any(elosEventDeepCopy, to);
    will_return(elosEventDeepCopy, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(safuVecPush, 0);
    expect_value(__wrap_safuVecPush, vec, &vector);
    expect_any(__wrap_safuVecPush, element);
    will_return(__wrap_safuVecPush, res);

    retval = elosEventVectorPushDeepCopy(&vector, &event);
    assert_int_equal(retval, res);
}

int elosTestElosEventVectorPushDeepCopyMockSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorPushDeepCopyMockTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorPushDeepCopyMock(UNUSED void **state) {
    _testMock(SAFU_RESULT_OK);
    _testMock(SAFU_RESULT_FAILED);
}
