// SPDX-License-Identifier: MIT

#include "mock_eventfilternode.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventFilterNodeInitialize, safuResultE_t, elosEventFilterNode_t *eventFilterNode,
               elosEventFilterNodeParam_t const *param) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeInitialize)) {
        check_expected_ptr(eventFilterNode);
        check_expected_ptr(param);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeInitialize)(eventFilterNode, param);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNodeDeleteMembers, safuResultE_t, elosEventFilterNode_t *eventFilterNode) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeDeleteMembers)) {
        check_expected_ptr(eventFilterNode);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeDeleteMembers)(eventFilterNode);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNodePush, safuResultE_t, elosEventFilterNode_t *eventFilterNode,
               char const **filterString, size_t filterStringCount) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodePush)) {
        check_expected_ptr(eventFilterNode);
        check_expected_ptr(filterString);
        check_expected(filterStringCount);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodePush)(eventFilterNode, filterString, filterStringCount);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventFilterNodeProcess, safuResultE_t, elosEventFilterNode_t *eventFilterNode,
               elosEventFilterStack_t const *filterStack, elosEvent_t const *event) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventFilterNodeProcess)) {
        check_expected_ptr(eventFilterNode);
        check_expected_ptr(filterStack);
        check_expected_ptr(event);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventFilterNodeProcess)(eventFilterNode, filterStack, event);
    }

    return result;
}
