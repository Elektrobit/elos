// SPDX-License-Identifier: MIT
#include <elos/eventfilter/eventfilter.h>
#include <elos/eventfilter/vector.h>
#include <elos/rpnfilter/rpnfilter.h>
#include <safu/common.h>
#include <safu/vector.h>

#include "elosStorageBackendAccepts_utest.h"

static const struct _TestRow {
    safuResultE_t expectedResult;
    elosEvent_t event;
    const char *filterStrings[3];
} elosTestTable[] = {
    {
        .expectedResult = SAFU_RESULT_OK,
        .event = {.messageCode = 42, .severity = ELOS_SEVERITY_FATAL},
        .filterStrings = {"1 1 EQ", NULL},
    },
    {
        .expectedResult = SAFU_RESULT_NOT_FOUND,
        .event = {.messageCode = 42, .severity = ELOS_SEVERITY_FATAL},
        .filterStrings = {".event.messageCode 0 EQ", NULL},
    },
    {
        .expectedResult = SAFU_RESULT_OK,
        .event = {.messageCode = 42, .severity = ELOS_SEVERITY_FATAL},
        .filterStrings = {".event.messageCode 0 EQ", ".event.severity 1 EQ", NULL},
    },
    {
        .expectedResult = SAFU_RESULT_OK,
        .event = {.messageCode = 42, .severity = ELOS_SEVERITY_FATAL},
        .filterStrings = {".event.messageCode 42 EQ", ".event.severity 0 EQ", NULL},
    },
    {
        .expectedResult = SAFU_RESULT_FAILED,
        .event = {.messageCode = 42, .severity = ELOS_SEVERITY_FATAL},
        .filterStrings = {"I will fail on filter execute", "I will fail on filter execute as well", NULL},
    },
};

int elosTestElosStorageBackendAcceptsSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosStorageBackendAcceptsSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosStorageBackendAcceptsSuccess(UNUSED void **state) {
    TEST("elosStorageBackendAccepts");
    SHOULD("%s", "test if SAFU_RESULT_OK is returned if filter rules for a backend matches given filters");

    for (size_t i = 0; i < ARRAY_SIZE(elosTestTable); i++) {
        PARAM("test row %zu", i);
        elosStorageBackend_t backend = {0};
        elosRpnFilterResultE_t filterResult = elosEventFilterVectorInit(&backend.filter, 1);
        assert_int_equal(filterResult, RPNFILTER_RESULT_OK);

        size_t j = 0;
        for (const char *filter = elosTestTable[i].filterStrings[j]; filter != NULL;
             j++, filter = elosTestTable[i].filterStrings[j]) {
            elosEventFilter_t eventFilter = {0};
            elosEventFilterParam_t const eventFilterParam = {.filterString = filter};
            filterResult = elosEventFilterCreate(&eventFilter, &eventFilterParam);
            assert_int_equal(filterResult, RPNFILTER_RESULT_OK);

            filterResult = elosEventFilterVectorPush(&backend.filter, &eventFilter);
            assert_int_equal(filterResult, RPNFILTER_RESULT_OK);
        }

        safuResultE_t result = elosStorageBackendAccepts(&backend, &elosTestTable[i].event);
        assert_int_equal(result, elosTestTable[i].expectedResult);

        elosEventFilterVectorDeleteMembers(&backend.filter);
    }
}
