// SPDX-License-Identifier: MIT

#include "mock_eventfiltermanager.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosEventFilterManagerInitialize, safuResultE_t, elosEventFilterManager_t *efm,
               elosEventFilterManagerParam_t const *param) {
    if (MOCK_IS_ACTIVE(elosEventFilterManagerInitialize)) {
        check_expected_ptr(efm);
        check_expected_ptr(param);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventFilterManagerInitialize)(efm, param);
}

MOCK_FUNC_BODY(elosEventFilterManagerDeleteMembers, safuResultE_t, elosEventFilterManager_t *efm) {
    if (MOCK_IS_ACTIVE(elosEventFilterManagerDeleteMembers)) {
        check_expected_ptr(efm);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventFilterManagerDeleteMembers)(efm);
}

MOCK_FUNC_BODY(elosEventFilterManagerNodeCreate, safuResultE_t, elosEventFilterManager_t *efm,
               char const **filterStrings, size_t filterStringCount, elosEventQueueId_t eqId,
               elosEventFilterNodeId_t *efnId) {
    if (MOCK_IS_ACTIVE(elosEventFilterManagerNodeCreate)) {
        check_expected_ptr(efm);
        check_expected_ptr(filterStrings);
        check_expected(filterStringCount);
        check_expected(eqId);
        check_expected_ptr(efnId);
        if (efnId != NULL) {
            *efnId = mock_type(elosEventFilterNodeId_t);
        }
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventFilterManagerNodeCreate)(efm, filterStrings, filterStringCount, eqId, efnId);
}

MOCK_FUNC_BODY(elosEventFilterManagerNodeRemove, safuResultE_t, elosEventFilterManager_t *efm,
               elosEventFilterNodeId_t efnId) {
    if (MOCK_IS_ACTIVE(elosEventFilterManagerNodeRemove)) {
        check_expected_ptr(efm);
        check_expected(efnId);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventFilterManagerNodeRemove)(efm, efnId);
}

MOCK_FUNC_BODY(elosEventFilterManagerNodeRemoveByEventQueueId, safuResultE_t, elosEventFilterManager_t *efm,
               elosEventQueueId_t eqId) {
    if (MOCK_IS_ACTIVE(elosEventFilterManagerNodeRemoveByEventQueueId)) {
        check_expected_ptr(efm);
        check_expected(eqId);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventFilterManagerNodeRemoveByEventQueueId)(efm, eqId);
}

MOCK_FUNC_BODY(elosEventFilterManagerProcess, safuResultE_t, elosEventFilterManager_t *efm, elosEvent_t const *event) {
    if (MOCK_IS_ACTIVE(elosEventFilterManagerProcess)) {
        check_expected_ptr(efm);
        check_expected_ptr(event);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosEventFilterManagerProcess)(efm, event);
}
