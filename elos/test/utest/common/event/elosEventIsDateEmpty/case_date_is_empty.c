// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "elosEventIsDateEmpty_utest.h"

void elosTestElosEventIsDateEmptyIsEmpty(UNUSED void **state) {
    TEST("elosEventIsDateEmpty");
    SHOULD("%s", "return true if both, tv.sec and tv.nsec are 0.");
    elosEvent_t testEvent = {.date = {.tv_sec = 0, .tv_nsec = 0}};
    assert_true(elosEventIsDateEmpty(&testEvent));
}
