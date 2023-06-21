// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "elosEventNew_utest.h"
#include "safu/mock_safu.h"

void elosTestElosEventNewExterrAllocMem(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEvent_t *event;

    TEST("elosEventNew");
    SHOULD("%s", "receive error while allocating memory for an elosEvent_t");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosEvent_t));
    will_return(__wrap_safuAllocMem, NULL);

    result = elosEventNew(&event);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
