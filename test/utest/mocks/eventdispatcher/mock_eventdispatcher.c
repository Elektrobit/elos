// SPDX-License-Identifier: MIT

#include "mock_eventdispatcher.h"

MOCK_FUNC_BODY(elosEventDispatcherInitialize, safuResultE_t, elosEventDispatcher_t *eventDispatcher,
               elosEventDispatcherParam_t const *const param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventDispatcherInitialize)) {
        check_expected_ptr(eventDispatcher);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventDispatcherInitialize)(eventDispatcher, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventDispatcherNew, safuResultE_t, elosEventDispatcher_t **eventDispatcherPtr,
               elosEventDispatcherParam_t const *const param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventDispatcherNew)) {
        check_expected_ptr(eventDispatcherPtr);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventDispatcherNew)(eventDispatcherPtr, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventDispatcherDeleteMembers, safuResultE_t, elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventDispatcherDeleteMembers)) {
        check_expected_ptr(eventDispatcher);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventDispatcherDeleteMembers)(eventDispatcher);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventDispatcherDelete, safuResultE_t, elosEventDispatcher_t **eventDispatcherPtr) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventDispatcherDelete)) {
        check_expected_ptr(eventDispatcherPtr);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventDispatcherDelete)(eventDispatcherPtr);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventDispatcherStart, safuResultE_t, elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventDispatcherStart)) {
        check_expected_ptr(eventDispatcher);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventDispatcherStart)(eventDispatcher);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventDispatcherStop, safuResultE_t, elosEventDispatcher_t *eventDispatcher) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventDispatcherStop)) {
        check_expected_ptr(eventDispatcher);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventDispatcherStop)(eventDispatcher);
    }

    return result;
}
