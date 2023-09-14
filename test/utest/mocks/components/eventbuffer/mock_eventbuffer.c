// SPDX-License-Identifier: MIT

#include "mock_eventbuffer.h"

MOCK_FUNC_BODY(elosEventBufferInitialize, safuResultE_t, elosEventBuffer_t *eventBuffer,
               elosEventBufferParam_t const *const param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventBufferInitialize)) {
        check_expected_ptr(eventBuffer);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventBufferInitialize)(eventBuffer, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventBufferNew, safuResultE_t, elosEventBuffer_t **eventBufferPtr,
               elosEventBufferParam_t const *const param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventBufferNew)) {
        check_expected_ptr(eventBufferPtr);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventBufferNew)(eventBufferPtr, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventBufferDeleteMembers, safuResultE_t, elosEventBuffer_t *eventBuffer) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventBufferDeleteMembers)) {
        check_expected_ptr(eventBuffer);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventBufferDeleteMembers)(eventBuffer);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventBufferDelete, safuResultE_t, elosEventBuffer_t **eventBufferPtr) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventBufferDelete)) {
        check_expected_ptr(eventBufferPtr);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventBufferDelete)(eventBufferPtr);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventBufferWrite, safuResultE_t, elosEventBuffer_t *eventBuffer, elosEvent_t const *const event) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventBufferWrite)) {
        check_expected_ptr(eventBuffer);
        check_expected_ptr(event);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventBufferWrite)(eventBuffer, event);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventBufferRead, safuResultE_t, elosEventBuffer_t *eventBuffer, size_t priority,
               elosEventPtrVector_t *eventPtrVector, size_t *elementsWritten) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventBufferRead)) {
        check_expected_ptr(eventBuffer);
        check_expected(priority);
        check_expected_ptr(eventPtrVector);
        check_expected_ptr(elementsWritten);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventBufferRead)(eventBuffer, priority, eventPtrVector, elementsWritten);
    }

    return result;
}
