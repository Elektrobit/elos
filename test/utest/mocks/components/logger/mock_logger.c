// SPDX-License-Identifier: MIT

#include "mock_logger.h"

#include <stdint.h>

MOCK_FUNC_BODY(elosLoggerInitialize, safuResultE_t, elosLogger_t *logger) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosLoggerInitialize)) {
        check_expected_ptr(logger);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosLoggerInitialize)(logger);
    }

    return result;
}

MOCK_FUNC_BODY(elosLoggerDeleteMembers, safuResultE_t, elosLogger_t *logger) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosLoggerDeleteMembers)) {
        check_expected_ptr(logger);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosLoggerDeleteMembers)(logger);
    }

    return result;
}

MOCK_FUNC_BODY(elosLoggerGetDefaultLogger, safuResultE_t, elosLogger_t **logger) {
    safuResultE_t result;

    if (MOCK_IS_ACTIVE(elosLoggerGetDefaultLogger)) {
        check_expected_ptr(logger);
        *logger = mock_ptr_type(elosLogger_t *);
        result = mock_type(safuResultE_t);
    } else {
        result = MOCK_FUNC_REAL(elosLoggerGetDefaultLogger)(logger);
    }

    return result;
}

MOCK_FUNC_BODY(elosLog, void, elosEventMessageCodeE_t messageCode, elosSeverityE_t severity, uint64_t classification,
               const char *logMessage) {
    if (MOCK_IS_ACTIVE(elosLog)) {
        check_expected(messageCode);
        check_expected(severity);
        check_expected(classification);
        check_expected_ptr(logMessage);
    } else {
        MOCK_FUNC_REAL(elosLog)(messageCode, severity, classification, logMessage);
    }
}
