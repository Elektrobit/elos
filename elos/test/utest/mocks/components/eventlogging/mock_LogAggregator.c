// SPDX-License-Identifier: MIT

#include "mock_LogAggregator.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosLogAggregatorStart, safuResultE_t, elosLogAggregator_t *logAggregator,
               elosLogAggregatorParam_t const *param) {
    if (MOCK_IS_ACTIVE(elosLogAggregatorStart)) {
        check_expected_ptr(logAggregator);
        check_expected_ptr(param);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosLogAggregatorStart)(logAggregator, param);
}

MOCK_FUNC_BODY(elosLogAggregatorAdd, safuResultE_t, elosLogAggregator_t *logAggregator, const elosEvent_t *event) {
    if (MOCK_IS_ACTIVE(elosLogAggregatorAdd)) {
        check_expected_ptr(logAggregator);
        check_expected_ptr(event);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosLogAggregatorAdd)(logAggregator, event);
}

MOCK_FUNC_BODY(elosLogAggregatorShutdown, safuResultE_t, elosLogAggregator_t *logAggregator) {
    if (MOCK_IS_ACTIVE(elosLogAggregatorShutdown)) {
        check_expected_ptr(logAggregator);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosLogAggregatorShutdown)(logAggregator);
}

MOCK_FUNC_BODY(elosLogAggregatorFindEvents, safuResultE_t, elosLogAggregator_t *logAggregator, const char *rule,
               safuVec_t *events) {
    if (MOCK_IS_ACTIVE(elosLogAggregatorFindEvents)) {
        check_expected_ptr(logAggregator);
        check_expected_ptr(rule);
        check_expected_ptr(events);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosLogAggregatorFindEvents)(logAggregator, rule, events);
}
