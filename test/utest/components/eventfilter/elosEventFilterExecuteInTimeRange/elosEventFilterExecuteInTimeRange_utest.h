// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENT_FILTER_EXECUTE_IN_TIME_RANGE_UTEST_H
#define ELOS_EVENT_FILTER_EXECUTE_IN_TIME_RANGE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/rpnfilter/rpnfilter.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>

#include "elos/eventfilter/eventfilter.h"

#define MAX_TEST_ENTRIES 32

typedef struct elosTestExecuteEntry {
    struct timespec newest;
    struct timespec oldest;
    elosRpnFilterResultE_t expected;
    elosEvent_t event;
} elosTestExecuteEntry_t;

typedef struct elosTestExecuteSet {
    char const *filterString;
    elosEventFilter_t *filter;
    uint32_t entryCount;
    elosTestExecuteEntry_t entries[MAX_TEST_ENTRIES];
} elosTestExecuteSet_t;

typedef struct elosTestData {
    uint32_t entryCount;
    elosTestExecuteSet_t executionSets[MAX_TEST_ENTRIES];
} elosTestData_t;

/****************************************************************
 * build the event filter and allocate the test data on the heap
 * to be managed by setup/teardown functions
 ****************************************************************/
safuResultE_t elosBuildTestData(elosTestData_t *testData, elosTestData_t **heapTestData);
/****************************************************************
 * frees the test data
 ****************************************************************/
safuResultE_t elosFreeTestData(elosTestData_t **testData);

/****************************************************************
 * run a single test for a specific filter, timerange and event combination
 ****************************************************************/
void elosRunSingleEntryTest(elosEventFilter_t *filter, elosTestExecuteEntry_t *testEntry);
/****************************************************************
 * Run all tests for a filter
 ****************************************************************/
void elosRunSingleFilterTests(elosTestExecuteSet_t *testSet);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventFilterExecuteInTimeRangeSuccessOutOfRange)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventFilterExecuteInTimeRangeSuccessInRange)

#endif /* ELOS_EVENT_FILTER_EXECUTE_IN_TIME_RANGE_UTEST_H */
