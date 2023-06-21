// SPDX-License-Identifier: MIT

#include "mock_eventqueuemanager.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventQueueManagerInitialize, safuResultE_t, elosEventQueueManager_t *eqm,
               elosEventQueueManagerParam_t const *param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerInitialize)) {
        check_expected_ptr(eqm);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerInitialize)(eqm, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerDeleteMembers, safuResultE_t, elosEventQueueManager_t *eqm) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerDeleteMembers)) {
        check_expected_ptr(eqm);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerDeleteMembers)(eqm);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerEntryNew, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t *eqId) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerEntryNew)) {
        check_expected_ptr(eqm);
        check_expected_ptr(eqId);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerEntryNew)(eqm, eqId);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerEntryDelete, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t eqId) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerEntryDelete)) {
        check_expected_ptr(eqm);
        check_expected(eqId);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerEntryDelete)(eqm, eqId);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerEntrySetLimit, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
               elosEventQueueLimitEvents_t limitEvents, elosEventQueueLimitTime_t limitTime) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerEntrySetLimit)) {
        check_expected_ptr(eqm);
        check_expected(eqId);
        check_expected(limitEvents);
        check_expected(limitTime.tv_sec);
        check_expected(limitTime.tv_nsec);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerEntrySetLimit)(eqm, eqId, limitEvents, limitTime);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerEntryGetLimit, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
               elosEventQueueLimitEvents_t *limitEvents, elosEventQueueLimitTime_t *limitTime) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerEntryGetLimit)) {
        check_expected_ptr(eqm);
        check_expected(eqId);
        check_expected_ptr(limitEvents);
        check_expected_ptr(limitTime);
        if (limitEvents != NULL) {
            *limitEvents = *mock_ptr_type(elosEventQueueLimitEvents_t *);
        }
        if (limitTime != NULL) {
            *limitTime = *mock_ptr_type(elosEventQueueLimitTime_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerEntryGetLimit)(eqm, eqId, limitEvents, limitTime);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerEntryGet, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
               elosEventQueue_t **eventQueue) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerEntryGet)) {
        check_expected_ptr(eqm);
        check_expected(eqId);
        check_expected_ptr(eventQueue);
        if (eventQueue != NULL) {
            *eventQueue = mock_ptr_type(elosEventQueue_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerEntryGet)(eqm, eqId, eventQueue);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerEntryRead, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
               elosEventVector_t **eventVector) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerEntryRead)) {
        check_expected_ptr(eqm);
        check_expected(eqId);
        check_expected_ptr(eventVector);
        if (eventVector != NULL) {
            *eventVector = mock_ptr_type(elosEventVector_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerEntryRead)(eqm, eqId, eventVector);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueManagerEntryPush, safuResultE_t, elosEventQueueManager_t *eqm, elosEventQueueId_t eqId,
               elosEvent_t *event) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueManagerEntryPush)) {
        check_expected_ptr(eqm);
        check_expected(eqId);
        check_expected_ptr(event);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueManagerEntryPush)(eqm, eqId, event);
    }

    return result;
}
