// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "elosEventNew_utest.h"
#include "safu/mock_safu.h"

void elosTestElosEventNewSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEvent_t mockEvent = {0};
    elosEvent_t *event = &mockEvent;

    TEST("elosEventNew");
    SHOULD("%s", "allocate successfully an elosEvent_t");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosEvent_t));
    will_return(__wrap_safuAllocMem, event);

    result = elosEventNew(&event);

    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(event->date.tv_sec, 0);
    assert_int_equal(event->date.tv_nsec, 0);
    assert_null(event->source.appName);
    assert_null(event->source.fileName);
    assert_int_equal(event->source.pid, 0);
    assert_int_equal(event->severity, 0);
    assert_null(event->hardwareid);
    assert_int_equal(event->classification, 0);
    assert_int_equal(event->messageCode, 0);
    assert_null(event->payload);
}
