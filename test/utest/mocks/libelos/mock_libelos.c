// SPDX-License-Identifier: MIT

#include "mock_libelos.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosConnectTcpip, safuResultE_t, const char *ip, uint16_t port, elosSession_t **session) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosConnectTcpip)) {
        check_expected_ptr(ip);
        check_expected(port);
        check_expected_ptr(session);
        if (session != NULL) {
            *session = mock_type(elosSession_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosConnectTcpip)(ip, port, session);
    }

    return result;
}

MOCK_FUNC_BODY(elosDisconnect, safuResultE_t, elosSession_t *session) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosDisconnect)) {
        check_expected_ptr(session);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosDisconnect)(session);
    }

    return result;
}

MOCK_FUNC_BODY(elosGetVersion, safuResultE_t, elosSession_t *session, const char **version) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosGetVersion)) {
        check_expected_ptr(session);
        check_expected_ptr(version);
        if (version != NULL) {
            *version = mock_type(const char *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosGetVersion)(session, version);
    }

    return result;
}

MOCK_FUNC_BODY(elosLogFindEvent, safuResultE_t, elosSession_t *session, const char *filterRule,
               elosEventVector_t **vector) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosLogFindEvent)) {
        check_expected_ptr(session);
        check_expected_ptr(filterRule);
        check_expected_ptr(vector);
        if (vector != NULL) {
            *vector = mock_type(elosEventVector_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosLogFindEvent)(session, filterRule, vector);
    }

    return result;
}

MOCK_FUNC_BODY(elosSessionValid, bool, elosSession_t const *session) {
    bool result;

    if (MOCK_IS_ACTIVE(elosSessionValid)) {
        check_expected_ptr(session);
        result = mock_type(bool);
    } else {
        result = MOCK_FUNC_REAL(elosSessionValid)(session);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventSubscribe, safuResultE_t, elosSession_t *session, char const **filterRuleArray,
               size_t filterRuleArraySize, elosEventQueueId_t *eventQueueId) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventSubscribe)) {
        check_expected_ptr(session);
        check_expected_ptr(filterRuleArray);
        check_expected(filterRuleArraySize);
        check_expected_ptr(eventQueueId);
        if (eventQueueId != NULL) {
            *eventQueueId = mock_type(elosEventQueueId_t);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventSubscribe)(session, filterRuleArray, filterRuleArraySize, eventQueueId);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventUnsubscribe, safuResultE_t, elosSession_t *session, elosEventQueueId_t eventQueueId) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventUnsubscribe)) {
        check_expected_ptr(session);
        check_expected_ptr(eventQueueId);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventUnsubscribe)(session, eventQueueId);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventPublish, safuResultE_t, elosSession_t *session, const elosEvent_t *event) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventPublish)) {
        check_expected_ptr(session);
        check_expected_ptr(event);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventPublish)(session, event);
    }

    return result;
}

MOCK_FUNC_BODY(elosEventQueueRead, safuResultE_t, elosSession_t *session, elosEventQueueId_t eventQueueId,
               elosEventVector_t **eventVector) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosEventQueueRead)) {
        check_expected_ptr(session);
        check_expected(eventQueueId);
        check_expected_ptr(eventVector);
        if (eventVector != NULL) {
            *eventVector = mock_type(elosEventVector_t *);
        }
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosEventQueueRead)(session, eventQueueId, eventVector);
    }

    return result;
}
