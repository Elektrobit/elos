// SPDX-License-Identifier: MIT

#include "elosEventSourceDelete_utest.h"

int elosTestElosEventSourceDeleteInitializedSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceDeleteInitializedSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceDeleteInitializedSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventSource_t *source;

    TEST("elosEventSourceDelete");
    SHOULD("%s", "free an initialized elosEventSource_t");

    result = elosEventSourceNew(&source);
    assert_non_null(source);
    assert_int_equal(result, SAFU_RESULT_OK);

    source->pid = 123;
    source->appName = strdup("appNameTest");
    assert_non_null(source->appName);
    source->fileName = strdup("fileNameTest");
    assert_non_null(source->fileName);

    elosEventSourceDelete(source);
}
