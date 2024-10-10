// SPDX-License-Identifier: MIT
#include <elos/event/event.h>
#include <elos/eventfilter/eventfilter_types.h>
#include <elos/rpnfilter/rpnfilter_types.h>
#include <safu/result.h>

#include "elosEventFilterExecuteInTimeRange_utest.h"

int elosTestElosEventFilterExecuteInTimeRangeSuccessInRangeSetup(void **state) {
    // clang-format off
    elosTestData_t testData = {
        .entryCount = 2,
        .executionSets = {
            {
                .filterString = ".event.payload '' STRCMP",
                .entryCount = 4,
                .entries = {
                    {
                        .newest = {.tv_sec = 100000, .tv_nsec = 12},
                        .oldest = {.tv_sec = 12, .tv_nsec = 136},
                        .expected = RPNFILTER_RESULT_MATCH,
                        .event = {.date = {.tv_sec = 75, .tv_nsec = 8}},
                    },{
                        .newest = {.tv_sec = 100000, .tv_nsec = 12},
                        .oldest = {.tv_sec = 12, .tv_nsec = 136},
                        .expected = RPNFILTER_RESULT_NO_MATCH,
                        .event = {.date = {.tv_sec = 75, .tv_nsec = 8}, .payload = "Some non empty payload"},
                    },{
                        .newest = {.tv_sec = 100000, .tv_nsec = 12},
                        .oldest = {.tv_sec = 0, .tv_nsec = 0},
                        .expected = RPNFILTER_RESULT_MATCH,
                        .event = {.date = {.tv_sec = 75, .tv_nsec = 8}, .payload = ""},
                    },{
                        .newest = {.tv_sec = 0, .tv_nsec = 0},
                        .oldest = {.tv_sec = 0, .tv_nsec = 0},
                        .expected = RPNFILTER_RESULT_NO_MATCH,
                        .event = {.date = {.tv_sec = 75, .tv_nsec = 8}, .payload = "Another non empty payload"},
                    },
                }
            },{
                .filterString = ".event.messageCode 2000 GE",
                .entryCount = 3,
                .entries = {
                    {
                        .newest = {.tv_sec = 100000, .tv_nsec = 12},
                        .oldest = {.tv_sec = 12, .tv_nsec = 136},
                        .expected = RPNFILTER_RESULT_MATCH,
                        .event = {.date = {.tv_sec = 83, .tv_nsec = 945}, .messageCode = ELOS_MSG_CODE_PROCESS_CREATED},
                    },{
                        .newest = {.tv_sec = 100007, .tv_nsec = 14},
                        .oldest = {.tv_sec = 6, .tv_nsec = 256},
                        .expected = RPNFILTER_RESULT_NO_MATCH,
                        .event = {.date = {.tv_sec = 83, .tv_nsec = 945}, .messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION},
                    },{
                        .newest = {.tv_sec = 0, .tv_nsec = 0},
                        .oldest = {.tv_sec = 12, .tv_nsec = 136},
                        .expected = RPNFILTER_RESULT_MATCH,
                        .event = {.date = {.tv_sec = 83, .tv_nsec = 945}, .messageCode = ELOS_MSG_CODE_PROCESS_CREATED},
                    },
                }
            },
        }
    };
    // clang-format on
    safuResultE_t result = elosBuildTestData(&testData, (elosTestData_t **)state);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventFilterExecuteInTimeRangeSuccessInRangeTeardown(void **state) {
    safuResultE_t result = elosFreeTestData((elosTestData_t **)state);
    assert_int_equal(result, SAFU_RESULT_OK);
    return 0;
}

void elosTestElosEventFilterExecuteInTimeRangeSuccessInRange(void **state) {
    TEST("elosTestElosEventFilterExecuteInTimeRangeSuccessInRange");
    SHOULD("Successfully check for events that are in the timerange");
    elosTestData_t *testData = *state;
    for (uint32_t i = 0; i < testData->entryCount; i++) {
        elosRunSingleFilterTests(&testData->executionSets[i]);
    }
}
