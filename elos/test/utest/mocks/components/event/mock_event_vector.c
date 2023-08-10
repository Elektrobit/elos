// SPDX-License-Identifier: MIT

#include "mock_event_vector.h"

MOCK_FUNC_BODY(elosEventVectorNew, safuResultE_t, elosEventVector_t **eventVector, uint32_t elements) {
    if (MOCK_IS_ACTIVE(elosEventVectorNew)) {
        check_expected_ptr(eventVector);
        check_expected(elements);
        if (eventVector != NULL) {
            *eventVector = mock_ptr_type(elosEventVector_t *);
        }
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventVectorNew)(eventVector, elements);
}

MOCK_FUNC_BODY(elosEventVectorInitialize, safuResultE_t, elosEventVector_t *eventVector, uint32_t elements) {
    if (MOCK_IS_ACTIVE(elosEventVectorInitialize)) {
        check_expected_ptr(eventVector);
        check_expected(elements);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventVectorInitialize)(eventVector, elements);
}

MOCK_FUNC_BODY(elosEventVectorToJsonArray, safuResultE_t, elosEventVector_t *eventVector, json_object **jEventVector) {
    if (MOCK_IS_ACTIVE(elosEventVectorToJsonArray)) {
        check_expected_ptr(eventVector);
        check_expected_ptr(jEventVector);
        if (jEventVector != NULL) {
            *jEventVector = mock_ptr_type(json_object *);
        }
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventVectorToJsonArray)(eventVector, jEventVector);
}

MOCK_FUNC_BODY(elosEventVectorToJsonObject, safuResultE_t, elosEventVector_t *eventVector, json_object **eventVecJobj) {
    if (MOCK_IS_ACTIVE(elosEventVectorToJsonObject)) {
        check_expected_ptr(eventVector);
        check_expected_ptr(eventVecJobj);
        if (eventVecJobj != NULL) {
            *eventVecJobj = mock_ptr_type(json_object *);
        }
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventVectorToJsonObject)(eventVector, eventVecJobj);
}

MOCK_FUNC_BODY(elosEventVectorFromJsonObject, safuResultE_t, json_object *eventVecJobj,
               elosEventVector_t **eventVector) {
    if (MOCK_IS_ACTIVE(elosEventVectorFromJsonObject)) {
        check_expected_ptr(eventVecJobj);
        check_expected_ptr(eventVector);
        if (eventVector != NULL) {
            *eventVector = mock_ptr_type(elosEventVector_t *);
        }
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventVectorFromJsonObject)(eventVecJobj, eventVector);
}

MOCK_FUNC_BODY(elosEventVectorFromJsonArray, safuResultE_t, json_object const *jEventArray,
               elosEventVector_t **eventVector) {
    if (MOCK_IS_ACTIVE(elosEventVectorFromJsonArray)) {
        check_expected_ptr(jEventArray);
        check_expected_ptr(eventVector);
        if (eventVector != NULL) {
            *eventVector = mock_ptr_type(elosEventVector_t *);
        }
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventVectorFromJsonArray)(jEventArray, eventVector);
}

MOCK_FUNC_BODY(elosEventVectorPush, safuResultE_t, elosEventVector_t *eventVector, elosEvent_t const *event) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventVectorPush)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventVectorPush)(eventVector, event);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventVector);
            check_expected_ptr(event);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventVectorPush)(eventVector, event);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventVectorPushDeepCopy, safuResultE_t, elosEventVector_t *eventVector, elosEvent_t const *event) {
    safuResultE_t result;

    switch (MOCK_GET_TYPE(elosEventVectorPushDeepCopy)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(elosEventVectorPushDeepCopy)(eventVector, event);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(eventVector);
            check_expected_ptr(event);
            result = mock_type(safuResultE_t);
            break;
        default:
            result = MOCK_FUNC_REAL(elosEventVectorPushDeepCopy)(eventVector, event);
            break;
    }

    return result;
}

MOCK_FUNC_BODY(elosEventVectorDeleteMembers, void, elosEventVector_t *eventVector) {
    if (MOCK_IS_ACTIVE(elosEventVectorDeleteMembers)) {
        check_expected_ptr(eventVector);
    } else {
        MOCK_FUNC_REAL(elosEventVectorDeleteMembers)(eventVector);
    }
}

MOCK_FUNC_BODY(elosEventVectorDelete, void, elosEventVector_t *eventVector) {
    if (MOCK_IS_ACTIVE(elosEventVectorDelete)) {
        check_expected_ptr(eventVector);
    } else {
        MOCK_FUNC_REAL(elosEventVectorDelete)(eventVector);
    }
}
