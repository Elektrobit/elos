// SPDX-License-Identifier: MIT

#include "mock_eventqueue.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventQueueNew, safuResultE_t, elosEventQueue_t **eventQueue, elosEventQueueParam_t const *param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueNew)) {
        check_expected_ptr(eventQueue);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueNew)(eventQueue, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueInitialize, safuResultE_t, elosEventQueue_t *eventQueue,
               elosEventQueueParam_t const *param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueInitialize)) {
        check_expected_ptr(eventQueue);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueInitialize)(eventQueue, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueDeleteMembers, void, elosEventQueue_t *eventQueue) {
    if (MOCK_IS_ACTIVE(elosEventQueueDeleteMembers)) {
        check_expected_ptr(eventQueue);
    } else {
        MOCK_FUNC_REAL(elosEventQueueDeleteMembers)(eventQueue);
    }
}

MOCK_FUNC_BODY(elosEventQueueDelete, void, elosEventQueue_t *eventQueue) {
    if (MOCK_IS_ACTIVE(elosEventQueueDelete)) {
        check_expected_ptr(eventQueue);
    } else {
        MOCK_FUNC_REAL(elosEventQueueDelete)(eventQueue);
    }
}

MOCK_FUNC_BODY(elosEventQueuePush, safuResultE_t, elosEventQueue_t *eventQueue, elosEvent_t const *event) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueuePush)) {
        check_expected_ptr(eventQueue);
        check_expected_ptr(event);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueuePush)(eventQueue, event);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueRead, safuResultE_t, elosEventQueue_t *eventQueue, elosEventVector_t **eventVector) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueRead)) {
        check_expected_ptr(eventQueue);
        check_expected_ptr(eventVector);
        if (eventVector != NULL) {
            *eventVector = mock_ptr_type(elosEventVector_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueRead)(eventQueue, eventVector);
    }

    return result;
}
