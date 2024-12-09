// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeInitialize_utest.h"

#define _FILTERID 42

int elosTestEloEventFilterNodeInitializeErrDoubleInitSetup(void **state) {
    *state = calloc(1, sizeof(elosTestState_t));

    return 0;
}

int elosTestEloEventFilterNodeInitializeErrDoubleInitTeardown(void **state) {
    elosTestState_t *test = *state;

    elosEventFilterNodeDeleteMembers(&test->efe);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeInitializeErrDoubleInit(void **state) {
    elosTestState_t *test = *state;
    char const *filterStrings[] = {".event.source.appName 'test' STRCMP"};
    uint32_t filterStringCount = ARRAY_SIZE(filterStrings);
    safuResultE_t result;
    elosEventFilterNodeParam_t const param = {
        .filterStrings = filterStrings,
        .filterStringCount = filterStringCount,
        .eventQueue = NULL,
        .id = _FILTERID,
    };

    TEST("elosEventFilterNodeInitialize");
    SHOULD("%s", "test correct behaviour of elosEventFilterNodeInitialize with double initialization");

    result = elosEventFilterNodeInitialize(&test->efe, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    result = elosEventFilterNodeInitialize(&test->efe, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
