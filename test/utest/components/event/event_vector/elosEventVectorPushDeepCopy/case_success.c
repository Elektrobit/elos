// SPDX-License-Identifier: MIT
#include <elos/event/event_vector.h>

#include "elosEventVectorPushDeepCopy_utest.h"

int elosTestElosEventVectorPushDeepCopySuccessSetup(void **state) {
    elosEventVector_t *vector = NULL;
    elosEventVectorNew(&vector, 1);
    *state = vector;
    return 0;
}

int elosTestElosEventVectorPushDeepCopySuccessTeardown(void **state) {
    elosEventVector_t *vector = *state;
    elosEventVectorDelete(vector);
    return 0;
}

void elosTestElosEventVectorPushDeepCopySuccess(void **state) {
    elosEvent_t event = {.messageCode = 4004, .payload = "An importand event to deep copy!"};
    elosEventVector_t *vector = *state;
    safuResultE_t retval;

    TEST("elosEventVectorPushDeepCopySuccess");
    SHOULD("%s", "successfully push a deep copy of an event into an event vector");

    retval = elosEventVectorPushDeepCopy(vector, &event);
    assert_int_equal(retval, SAFU_RESULT_OK);
    assert_int_equal(vector->elementCount, 1);

    elosEvent_t *resEvent = safuVecGetLast(vector);
    assert_int_equal(resEvent->messageCode, event.messageCode);
    assert_string_equal(resEvent->payload, event.payload);
    assert_ptr_not_equal(resEvent->payload, event.payload);
}
