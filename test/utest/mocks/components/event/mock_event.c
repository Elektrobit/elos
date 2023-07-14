// SPDX-License-Identifier: MIT

#include "mock_event.h"

#include <stdbool.h>

MOCK_FUNC_BODY(elosEventSerialize, safuResultE_t, char **toJsonString, const elosEvent_t *from) {
    if (MOCK_IS_ACTIVE(elosEventSerialize)) {
        check_expected_ptr(toJsonString);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventSerialize)(toJsonString, from);
}

MOCK_FUNC_BODY(elosEventDeepCopy, safuResultE_t, elosEvent_t *to, const elosEvent_t *from) {
    if (MOCK_IS_ACTIVE(elosEventDeepCopy)) {
        check_expected_ptr(to);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventDeepCopy)(to, from);
}

MOCK_FUNC_BODY(elosEventNew, safuResultE_t, elosEvent_t **event) {
    if (MOCK_IS_ACTIVE(elosEventNew)) {
        check_expected_ptr(event);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventNew)(event);
}

MOCK_FUNC_BODY(elosEventDeleteMembers, safuResultE_t, elosEvent_t *event) {
    if (MOCK_IS_ACTIVE(elosEventDeleteMembers)) {
        check_expected_ptr(event);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventDeleteMembers)(event);
}

MOCK_FUNC_BODY(elosEventDelete, safuResultE_t, elosEvent_t *event) {
    if (MOCK_IS_ACTIVE(elosEventDelete)) {
        check_expected_ptr(event);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventDelete)(event);
}

MOCK_FUNC_BODY(elosEventDeserialize, safuResultE_t, elosEvent_t *to, const char *fromJsonString) {
    if (MOCK_IS_ACTIVE(elosEventDeserialize)) {
        check_expected_ptr(to);
        check_expected_ptr(fromJsonString);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventDeserialize)(to, fromJsonString);
}

MOCK_FUNC_BODY(elosEventFromJsonObject, safuResultE_t, elosEvent_t *to, const json_object *from) {
    if (MOCK_IS_ACTIVE(elosEventFromJsonObject)) {
        check_expected_ptr(to);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventFromJsonObject)(to, from);
}

MOCK_FUNC_BODY(elosEventToJsonObject, safuResultE_t, json_object *to, const elosEvent_t *from) {
    if (MOCK_IS_ACTIVE(elosEventToJsonObject)) {
        check_expected_ptr(to);
        check_expected_ptr(from);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventToJsonObject)(to, from);
}

MOCK_FUNC_BODY(elosEventIsDateEmpty, bool, const elosEvent_t *const event) {
    if (MOCK_IS_ACTIVE(elosEventIsDateEmpty)) {
        check_expected_ptr(event);
        return mock_type(bool);
    }
    return MOCK_FUNC_REAL(elosEventIsDateEmpty)(event);
}
