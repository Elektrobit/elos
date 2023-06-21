// SPDX-License-Identifier: MIT
#include <safu/common.h>
#include <stdlib.h>

#include "cmocka_mocks/mock_libc.h"
#include "safuTimeGetLocalTime_utest.h"

/*
 * offset for 'struct tm' values as defined by
 * C99 standard (ISO/IEC 9899:1999): 7.23.1/3 Components of time (p: 338)
 * in bits/types/struct_tm.h
 */
#define SINCE_1900          1900
#define SINCE_JANUARY       1
#define SINCE_SUNDAY        1
#define SINCE_JANUARY_FIRST 1

typedef struct safuTestState {
    char *timezone;
} safuTestState_t;

static struct safuTestData {
    time_t testTimeStamp;
    struct tm expectedDate;
} safuTestData[] = {
    {
        .testTimeStamp = 1640995200,
        .expectedDate =
            {
                .tm_year = 2022 - SINCE_1900,
                .tm_mon = 1 - SINCE_JANUARY,
                .tm_wday = 7 - SINCE_SUNDAY,
                .tm_mday = 1,
                .tm_yday = 1 - SINCE_JANUARY_FIRST,
                .tm_hour = 0,
                .tm_min = 0,
                .tm_sec = 0,
                .tm_isdst = 0,
            },
    },
    {
        .testTimeStamp = 1646042948,
        .expectedDate =
            {
                .tm_year = 2022 - SINCE_1900,
                .tm_mon = 2 - SINCE_JANUARY,
                .tm_wday = 2 - SINCE_SUNDAY,
                .tm_mday = 28,
                .tm_yday = 59 - SINCE_JANUARY_FIRST,
                .tm_hour = 10,
                .tm_min = 9,
                .tm_sec = 8,
                .tm_isdst = 0,
            },
    },
    {
        .testTimeStamp = 1646129348,
        .expectedDate =
            {
                .tm_year = 2022 - SINCE_1900,
                .tm_mon = 3 - SINCE_JANUARY,
                .tm_wday = 3 - SINCE_SUNDAY,
                .tm_mday = 1,
                .tm_yday = 60 - SINCE_JANUARY_FIRST,
                .tm_hour = 10,
                .tm_min = 9,
                .tm_sec = 8,
                .tm_isdst = 0,
            },
    },
    {
        .testTimeStamp = 1653381265,
        .expectedDate =
            {
                .tm_year = 2022 - SINCE_1900,
                .tm_mon = 5 - SINCE_JANUARY,
                .tm_wday = 3 - SINCE_SUNDAY,
                .tm_mday = 24,
                .tm_yday = 144 - SINCE_JANUARY_FIRST,
                .tm_hour = 8,
                .tm_min = 34,
                .tm_sec = 25,
                .tm_isdst = 0,
            },
    },
};

int safuTestSafuTimeGetLocalTimeTimeReturnValidValueSetup(void **state) {
    safuTestState_t *testState = calloc(1, sizeof(safuTestState_t));
    if (testState == NULL) {
        return -1;
    }
    *state = testState;

    testState->timezone = getenv("TZ");
    setenv("TZ", "UTC", 1);
    tzset();
    return 0;
}

int safuTestSafuTimeGetLocalTimeTimeReturnValidValueTeardown(void **state) {
    safuTestState_t *testState = *state;
    if (testState->timezone == NULL) {
        unsetenv("TZ");
    } else {
        setenv("TZ", testState->timezone, 1);
    }
    tzset();
    free(testState);
    return 0;
}

static void _testSafuTimeGetLocalTimeTimeReturnValidValueParam(time_t testTimeStamp, const struct tm *expectedDate) {
    struct tm testTime = {0};

    PARAM("timestamp == %ld", testTimeStamp);

    MOCK_FUNC_AFTER_CALL(time, 0);

    expect_any(__wrap_time, timer);
    will_set_parameter(__wrap_time, timer, testTimeStamp);
    will_return(__wrap_time, testTimeStamp);

    safuResultE_t result = safuTimeGetLocalTime(&testTime);

    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(testTime.tm_year, expectedDate->tm_year);
    assert_int_equal(testTime.tm_mon, expectedDate->tm_mon);
    assert_int_equal(testTime.tm_wday, expectedDate->tm_wday);
    assert_int_equal(testTime.tm_mday, expectedDate->tm_mday);
    assert_int_equal(testTime.tm_yday, expectedDate->tm_yday);

    assert_int_equal(testTime.tm_hour, expectedDate->tm_hour);
    assert_int_equal(testTime.tm_min, expectedDate->tm_min);
    assert_int_equal(testTime.tm_sec, expectedDate->tm_sec);

    assert_int_equal(testTime.tm_isdst, expectedDate->tm_isdst);
}

void safuTestSafuTimeGetLocalTimeTimeReturnValidValue(UNUSED void **state) {
    TEST("safuTimeGetLocalTime");
    SHOULD("%s", "fill a provided struct tm with the current date and time obtained from time()");
    for (size_t i = 0; i < ARRAY_SIZE(safuTestData); i++) {
        _testSafuTimeGetLocalTimeTimeReturnValidValueParam(safuTestData[i].testTimeStamp,
                                                           &safuTestData[i].expectedDate);
    }
}
