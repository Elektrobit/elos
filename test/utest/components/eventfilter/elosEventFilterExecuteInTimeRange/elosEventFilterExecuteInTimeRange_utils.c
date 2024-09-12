// SPDX-License-Identifier: MIT
#include <elos/event/event.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>

#include "elosEventFilterExecuteInTimeRange_utest.h"

safuResultE_t elosBuildTestData(elosTestData_t *testData, elosTestData_t **heapTestData) {
    safuResultE_t result = SAFU_RESULT_OK;

    *heapTestData = safuAllocMem(NULL, sizeof(elosTestData_t));
    memcpy(*heapTestData, testData, sizeof(elosTestData_t));

    for (size_t i = 0; i < testData->entryCount; i++) {
        char *filterString = safuAllocMem(NULL, strlen(testData->executionSets[i].filterString) + 1);
        strcpy(filterString, testData->executionSets[i].filterString);
        (*heapTestData)->executionSets[i].filterString = filterString;

        elosEventFilterParam_t param = {.filterString = (*heapTestData)->executionSets[i].filterString};
        elosRpnFilterResultE_t filterRes = elosEventFilterNew(&(*heapTestData)->executionSets[i].filter);
        if (filterRes != RPNFILTER_RESULT_OK) {
            result = SAFU_RESULT_FAILED;
            break;
        }
        filterRes = elosEventFilterCreate((*heapTestData)->executionSets[i].filter, &param);
        if (filterRes != RPNFILTER_RESULT_OK) {
            result = SAFU_RESULT_FAILED;
        }

        for (size_t j = 0; j < testData->executionSets[i].entryCount; j++) {
            elosEventDeepCopy(&(*heapTestData)->executionSets[i].entries[j].event,
                              &testData->executionSets[i].entries[j].event);
        }
    }
    return result;
}

safuResultE_t elosFreeTestData(elosTestData_t **testData) {
    safuResultE_t result = SAFU_RESULT_OK;
    for (size_t i = 0; i < (*testData)->entryCount; i++) {
        elosRpnFilterResultE_t filterRes = elosRpnFilterDelete((*testData)->executionSets[i].filter);
        if (filterRes != RPNFILTER_RESULT_OK) {
            result = SAFU_RESULT_FAILED;
        }
        for (size_t j = 0; j < (*testData)->executionSets[i].entryCount; j++) {
            safuResultE_t tmpRes = elosEventDeleteMembers(&(*testData)->executionSets[i].entries[j].event);
            if (tmpRes != SAFU_RESULT_OK) {
                result = tmpRes;
            }
        }
        free((void *)(*testData)->executionSets[i].filterString);
    }
    free(*testData);
    *testData = NULL;
    return result;
}

void elosRunSingleEntryTest(elosEventFilter_t *filter, elosTestExecuteEntry_t *testEntry) {
    char *json = NULL;
    safuResultE_t serialRes = elosEventSerialize(&json, &testEntry->event);
    assert_int_equal(serialRes, SAFU_RESULT_OK);
    PARAM("  - TimeRange: [{%ld,%ld},{%ld,%ld}), Event: %s", testEntry->newest.tv_sec, testEntry->newest.tv_nsec,
          testEntry->oldest.tv_sec, testEntry->oldest.tv_nsec, json);
    free(json);
    elosRpnFilterResultE_t result =
        elosEventFilterExecuteInTimeRange(filter, NULL, &testEntry->newest, &testEntry->oldest, &testEntry->event);
    assert_int_equal(result, testEntry->expected);
}

void elosRunSingleFilterTests(elosTestExecuteSet_t *testSet) {
    PARAM("Filter: %s", testSet->filterString);
    for (uint32_t i = 0; i < testSet->entryCount; i++) {
        elosRunSingleEntryTest(testSet->filter, &testSet->entries[i]);
    }
}
