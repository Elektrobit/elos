// SPDX-License-Identifier: MIT
#include "elosEventDeepCopy_utest.h"

int elosTestElosEventDeepCopyNullInputSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeepCopyNullInputTeardown(UNUSED void **state) {
    return 0;
}

static void _runElosEventDeepCopyNullInput(elosEvent_t *to, const elosEvent_t *from) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (to == NULL) {
        PARAM("%s", "Destination event is null");
    } else {
        PARAM("%s", "Source event is null");
    }

    result = elosEventDeepCopy(to, from);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}

void elosTestElosEventDeepCopyNullInput(UNUSED void **state) {
    const elosEvent_t testSource = {.classification = 42};
    elosEvent_t testDest = {0};

    TEST("elosEventDeepCopy");
    SHOULD("%s", "return error upon receiving null input event");

    _runElosEventDeepCopyNullInput(NULL, &testSource);
    _runElosEventDeepCopyNullInput(&testDest, NULL);
}
