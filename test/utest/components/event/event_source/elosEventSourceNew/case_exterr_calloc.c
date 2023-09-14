// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/event/event_source.h"
#include "elosEventSourceNew_utest.h"

int elosTestElosEventSourceNewExterrCallocSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSourceNewExterrCallocTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSourceNewExterrCalloc(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEventSource_t *testEventSource = NULL;

    TEST("elosEventSourceNew");
    SHOULD("%s", "receive error while allocating memory for an elosEventSource_t");

    MOCK_FUNC_AFTER_CALL(calloc, 0);
    expect_any(__wrap_calloc, nmemb);
    expect_any(__wrap_calloc, size);
    will_return(__wrap_calloc, NULL);

    result = elosEventSourceNew(&testEventSource);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
