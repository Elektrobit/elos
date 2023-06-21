// SPDX-License-Identifier: MIT

#include <limits.h>

#include "elosClientManagerThreadGetFreeConnectionSlot_utest.h"

int elosTestElosClientManagerThreadGetFreeConnectionSlotRecalcTvsecSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosClientManagerThreadGetFreeConnectionSlotRecalcTvsecTeardown(UNUSED void **state) {
    return 0;
}

static void elosTestTimeCalculationParameters(elosClientManagerContext_t *context, struct timespec *ts,
                                              struct timespec *expectedTs, size_t idx) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    int slot = 0;

    PARAM("Nr %d: %ld seconds, %ld nano seconds", idx, ts->tv_sec, ts->tv_nsec);

    MOCK_FUNC_AFTER_CALL(clock_gettime, 0)
    expect_any(__wrap_clock_gettime, clock_id);
    expect_any(__wrap_clock_gettime, tp);
    will_set_parameter(__wrap_clock_gettime, tp, ts);
    will_return(__wrap_clock_gettime, 0);

    MOCK_FUNC_AFTER_CALL(sem_timedwait, 0)
    expect_any(__wrap_sem_timedwait, __sem);
    expect_memory(__wrap_sem_timedwait, __abstime, expectedTs, sizeof(ts));
    will_return(__wrap_sem_timedwait, 0);

    result = elosClientManagerThreadGetFreeConnectionSlot(context, &slot);
    assert_int_equal(result, SAFU_RESULT_OK);
}

void elosTestElosClientManagerThreadGetFreeConnectionSlotRecalcTvsec(void **state) {
    elosClientManagerContext_t *context = *state;

    struct timespec testTimes[] = {
        {.tv_sec = 0, .tv_nsec = 0},
        {.tv_sec = 0, .tv_nsec = TIMESPEC_1SEC_IN_NSEC - CONNECTION_SEMAPHORE_TIMEOUT_NSEC - 1},
        {.tv_sec = 0, .tv_nsec = TIMESPEC_1SEC_IN_NSEC},
        {.tv_sec = 0, .tv_nsec = 3 * TIMESPEC_1SEC_IN_NSEC},
        {.tv_sec = 0, .tv_nsec = LONG_MAX},

        {.tv_sec = LONG_MAX / 2, .tv_nsec = 0},
        {.tv_sec = LONG_MAX / 2, .tv_nsec = TIMESPEC_1SEC_IN_NSEC - CONNECTION_SEMAPHORE_TIMEOUT_NSEC - 1},
        {.tv_sec = LONG_MAX / 2, .tv_nsec = TIMESPEC_1SEC_IN_NSEC},
        {.tv_sec = LONG_MAX / 2, .tv_nsec = 3 * TIMESPEC_1SEC_IN_NSEC},
        {.tv_sec = LONG_MAX / 2, .tv_nsec = LONG_MAX},

        {.tv_sec = LONG_MAX, .tv_nsec = 0},
        {.tv_sec = LONG_MAX, .tv_nsec = TIMESPEC_1SEC_IN_NSEC - CONNECTION_SEMAPHORE_TIMEOUT_NSEC - 1},
        {.tv_sec = LONG_MAX, .tv_nsec = TIMESPEC_1SEC_IN_NSEC},
        {.tv_sec = LONG_MAX, .tv_nsec = 3 * TIMESPEC_1SEC_IN_NSEC},
        {.tv_sec = LONG_MAX, .tv_nsec = LONG_MAX},
    };

    /* When tv_nsec is larger than 1 second, the timeradd function always substractes only the value
    of one second from it and adds it to the tv_sec. Even if tv_nsec contains the value of multiple
    seconds.*/
    struct timespec timesExpected[] = {
        {.tv_sec = 0, .tv_nsec = CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = 0, .tv_nsec = TIMESPEC_1SEC_IN_NSEC - 1},
        {.tv_sec = 1, .tv_nsec = CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = 1, .tv_nsec = 2 * CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = 1, .tv_nsec = LONG_MAX - CONNECTION_SEMAPHORE_TIMEOUT_NSEC},

        {.tv_sec = LONG_MAX / 2, .tv_nsec = CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = LONG_MAX / 2, .tv_nsec = TIMESPEC_1SEC_IN_NSEC - 1},
        {.tv_sec = LONG_MAX / 2 + 1, .tv_nsec = CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = LONG_MAX / 2 + 1, .tv_nsec = 2 * CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = LONG_MAX / 2 + 1, .tv_nsec = LONG_MAX - CONNECTION_SEMAPHORE_TIMEOUT_NSEC},

        {.tv_sec = LONG_MAX, .tv_nsec = CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = LONG_MAX, .tv_nsec = TIMESPEC_1SEC_IN_NSEC - 1},
        {.tv_sec = LONG_MIN, .tv_nsec = CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = LONG_MIN, .tv_nsec = 2 * CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
        {.tv_sec = LONG_MIN, .tv_nsec = LONG_MAX - CONNECTION_SEMAPHORE_TIMEOUT_NSEC},
    };

    TEST("elosClientManagerThreadGetFreeConnectionSlot");
    SHOULD("%s", "test different time values");

    size_t i, numArrayElements = sizeof(testTimes) / sizeof(testTimes[0]);

    for (i = 0; i < numArrayElements; i++) {
        memset(context, '\0', sizeof(elosClientManagerContext_t));
        elosTestTimeCalculationParameters(context, &testTimes[i], &timesExpected[i], i + 1);
    }
}
