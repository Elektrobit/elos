// SPDX-License-Identifier: MIT

#include "elosEventSourceDelete_utest.h"

int elosTestElosEventSourceDeleteUninitializedSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeleteUninitializedSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeleteUninitializedSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventSource_t *source;

    TEST("elosEventSourceDelete");
    SHOULD("%s", "free an uninitialized elosEventSource_t");

    result = elosEventSourceNew(&source);
    assert_non_null(source);
    assert_int_equal(result, SAFU_RESULT_OK);
    memset(source, 0, sizeof(elosEventSource_t));

    elosEventSourceDelete(source);
}
