// SPDX-License-Identifier: MIT

#include "mock_ringbuffer.h"

MOCK_FUNC_BODY(safuRingBufferNew, safuResultE_t, safuRingBuffer_t **ringBuffer,
               safuRingBufferParam_t const *const param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(safuRingBufferNew)) {
        check_expected_ptr(ringBuffer);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(safuRingBufferNew)(ringBuffer, param);
    }

    return result;
}

MOCK_FUNC_BODY(safuRingBufferInitialize, safuResultE_t, safuRingBuffer_t *ringBuffer,
               safuRingBufferParam_t const *const param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(safuRingBufferInitialize)) {
        check_expected_ptr(ringBuffer);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(safuRingBufferInitialize)(ringBuffer, param);
    }

    return result;
}

MOCK_FUNC_BODY(safuRingBufferDeleteMembers, safuResultE_t, safuRingBuffer_t *ringBuffer) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(safuRingBufferDeleteMembers)) {
        check_expected_ptr(ringBuffer);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(safuRingBufferDeleteMembers)(ringBuffer);
    }

    return result;
}

MOCK_FUNC_BODY(safuRingBufferDelete, safuResultE_t, safuRingBuffer_t **ringBuffer) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(safuRingBufferDelete)) {
        check_expected_ptr(ringBuffer);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(safuRingBufferDelete)(ringBuffer);
    }

    return result;
}

MOCK_FUNC_BODY(safuRingBufferWrite, safuResultE_t, safuRingBuffer_t *ringBuffer, void *element) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(safuRingBufferWrite)) {
        check_expected_ptr(ringBuffer);
        check_expected_ptr(element);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(safuRingBufferWrite)(ringBuffer, element);
    }

    return result;
}

MOCK_FUNC_BODY(safuRingBufferRead, safuResultE_t, safuRingBuffer_t *ringBuffer, safuVec_t *elementVector,
               size_t *elementsWritten) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(safuRingBufferRead)) {
        check_expected_ptr(ringBuffer);
        check_expected_ptr(elementVector);
        check_expected_ptr(elementsWritten);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(safuRingBufferRead)(ringBuffer, elementVector, elementsWritten);
    }

    return result;
}
