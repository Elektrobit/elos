// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "elosEventIsDateEmpty_utest.h"

static void _testElosEventIsDateEmptyIsNotEmptyParam(const elosEvent_t *const event) {
    PARAM("tv_sec=%ld, tv_nsec=%ld", event->date.tv_sec, event->date.tv_nsec);

    assert_false(elosEventIsDateEmpty(event));
}

void elosTestElosEventIsDateEmptyIsNotEmpty(UNUSED void **state) {
    TEST("elosEventIsDateEmpty");
    SHOULD("%s", "return always false as long as tv_sec and tv_nsec not 0 at the same time.");

    elosEvent_t testEvents[] = {
        {.date = {.tv_sec = 1, .tv_nsec = 0}},
        {.date = {.tv_sec = 0, .tv_nsec = 1}},
        {.date = {.tv_sec = INT64_MAX, .tv_nsec = 0}},
        {.date = {.tv_sec = 0, .tv_nsec = INT64_MAX}},
        {.date = {.tv_sec = INT64_MAX, .tv_nsec = INT64_MAX}},
        {.date = {.tv_sec = INT64_MAX / 2, .tv_nsec = INT64_MAX / 2}},
        {.date = {.tv_sec = INT64_MIN, .tv_nsec = INT64_MIN}},
        {.date = {.tv_sec = INT64_MIN / 2, .tv_nsec = INT64_MIN / 2}},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testEvents); i++) {
        _testElosEventIsDateEmptyIsNotEmptyParam(&testEvents[i]);
    }
}
