// SPDX-License-Identifier: MIT
#include "elosEventVectorPush_utest.h"
#include "safu/common.h"
#include "safu/mock_vector.h"

static inline void _testMock(safuResultE_t res) {
    elosEvent_t event = {0};
    elosEventVector_t vector = {0};
    safuResultE_t retval;

    MOCK_FUNC_AFTER_CALL(safuVecPush, 0);
    expect_value(__wrap_safuVecPush, vec, &vector);
    expect_value(__wrap_safuVecPush, element, &event);
    will_return(__wrap_safuVecPush, res);

    retval = elosEventVectorPush(&vector, &event);
    assert_int_equal(retval, res);
}

int elosTestElosEventVectorPushMockSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorPushMockTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorPushMock(UNUSED void **state) {
    _testMock(SAFU_RESULT_OK);
    _testMock(SAFU_RESULT_FAILED);
}
