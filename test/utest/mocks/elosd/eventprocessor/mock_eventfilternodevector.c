// SPDX-License-Identifier: MIT

#include "mock_eventfilternodevector.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventFilterNodeVectorInitialize, safuResultE_t, elosEventFilterNodeVector_t *eventFilterNodeVector,
               uint32_t elements) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeVectorInitialize)) {
        check_expected_ptr(eventFilterNodeVector);
        check_expected(elements);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeVectorInitialize)(eventFilterNodeVector, elements);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNodeVectorDeleteMembers, safuResultE_t,
               elosEventFilterNodeVector_t *eventFilterNodeVector) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeVectorDeleteMembers)) {
        check_expected_ptr(eventFilterNodeVector);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeVectorDeleteMembers)(eventFilterNodeVector);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNodeVectorPushByParam, safuResultE_t, elosEventFilterNodeVector_t *eventFilterNodeVector,
               elosEventFilterNodeParam_t const *param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeVectorPushByParam)) {
        check_expected_ptr(eventFilterNodeVector);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeVectorPushByParam)(eventFilterNodeVector, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNodeVectorRemoveById, safuResultE_t, elosEventFilterNodeVector_t *eventFilterNodeVector,
               elosEventFilterNodeId_t id) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeVectorRemoveById)) {
        check_expected_ptr(eventFilterNodeVector);
        check_expected(id);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeVectorRemoveById)(eventFilterNodeVector, id);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNodeVectorProcess, safuResultE_t, elosEventFilterNodeVector_t *eventFilterNodeVector,
               elosEventFilterStack_t const *filterStack, elosEvent_t const *event) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeVectorProcess)) {
        check_expected_ptr(eventFilterNodeVector);
        check_expected_ptr(filterStack);
        check_expected_ptr(event);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeVectorProcess)(eventFilterNodeVector, filterStack, event);
    }

    return result;
}
