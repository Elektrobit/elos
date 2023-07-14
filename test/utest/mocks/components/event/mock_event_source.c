// SPDX-License-Identifier: MIT

#include "mock_event_source.h"

MOCK_FUNC_BODY(elosEventSourceNew, safuResultE_t, elosEventSource_t **eventSource) {
    if (MOCK_IS_ACTIVE(elosEventSourceNew)) {
        check_expected_ptr(*eventSource);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventSourceNew)(eventSource);
}

MOCK_FUNC_BODY(elosEventSourceDeleteMembers, void, elosEventSource_t *eventSource) {
    if (MOCK_IS_ACTIVE(elosEventSourceDeleteMembers)) {
        check_expected_ptr(eventSource);
    } else {
        MOCK_FUNC_REAL(elosEventSourceDeleteMembers)(eventSource);
    }
}

MOCK_FUNC_BODY(elosEventSourceDelete, void, elosEventSource_t *eventSource) {
    if (MOCK_IS_ACTIVE(elosEventSourceDelete)) {
        check_expected_ptr(eventSource);
    } else {
        MOCK_FUNC_REAL(elosEventSourceDelete)(eventSource);
    }
}

MOCK_FUNC_BODY(elosEventSourceSerialize, safuResultE_t, char **toJsonString, elosEventSource_t *from) {
    if (MOCK_IS_ACTIVE(elosEventSourceSerialize)) {
        check_expected_ptr(*toJsonString);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventSourceSerialize)(toJsonString, from);
}

MOCK_FUNC_BODY(elosEventSourceDeserialize, safuResultE_t, elosEventSource_t *to, const char *fromJsonString) {
    if (MOCK_IS_ACTIVE(elosEventSourceDeserialize)) {
        check_expected_ptr(to);
        check_expected_ptr(fromJsonString);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventSourceDeserialize)(to, fromJsonString);
}

MOCK_FUNC_BODY(elosEventSourceDeepCopy, safuResultE_t, elosEventSource_t *to, const elosEventSource_t *from) {
    if (MOCK_IS_ACTIVE(elosEventSourceDeepCopy)) {
        check_expected_ptr(to);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventSourceDeepCopy)(to, from);
}

MOCK_FUNC_BODY(elosEventSourceToJsonObject, safuResultE_t, json_object *to, const elosEventSource_t *from) {
    if (MOCK_IS_ACTIVE(elosEventSourceToJsonObject)) {
        check_expected_ptr(to);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventSourceToJsonObject)(to, from);
}

MOCK_FUNC_BODY(elosEventSourceFromJsonObject, safuResultE_t, elosEventSource_t *to, const json_object *from) {
    if (MOCK_IS_ACTIVE(elosEventSourceFromJsonObject)) {
        check_expected_ptr(to);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventSourceFromJsonObject)(to, from);
}
