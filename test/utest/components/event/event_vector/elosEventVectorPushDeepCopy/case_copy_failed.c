// SPDX-License-Identifier: MIT
#include "elosEventVectorPushDeepCopy_utest.h"
#include "mock_event.h"
#include "safu/mock_vector.h"

int elosTestElosEventVectorPushDeepCopyCopyFailedSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorPushDeepCopyCopyFailedTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorPushDeepCopyCopyFailed(UNUSED void **state) {
    elosEvent_t event = {0};
    elosEventVector_t vector = {0};
    safuResultE_t retval;

    MOCK_FUNC_AFTER_CALL(elosEventDeepCopy, 0);
    expect_any(elosEventDeepCopy, to);
    expect_value(elosEventDeepCopy, from, &event);
    will_return(elosEventDeepCopy, SAFU_RESULT_FAILED);

    retval = elosEventVectorPushDeepCopy(&vector, &event);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
