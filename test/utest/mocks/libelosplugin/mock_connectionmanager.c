// SPDX-License-Identifier: MIT

#include "mock_connectionmanager.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosConnectionManagerThreadGetFreeConnectionSlot, safuResultE_t, elosConnectionManager_t *ctx,
               int *slot) {
    if (MOCK_IS_ACTIVE(elosConnectionManagerThreadGetFreeConnectionSlot)) {
        check_expected_ptr(ctx);
        check_expected_ptr(slot);
        *slot = mock_type(int);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosConnectionManagerThreadGetFreeConnectionSlot)(ctx, slot);
}

MOCK_FUNC_BODY(elosConnectionManagerThreadWaitForIncomingConnection, safuResultE_t, elosConnectionManager_t *ctx,
               int slot) {
    if (MOCK_IS_ACTIVE(elosConnectionManagerThreadWaitForIncomingConnection)) {
        check_expected_ptr(ctx);
        check_expected(slot);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosConnectionManagerThreadWaitForIncomingConnection)(ctx, slot);
}

MOCK_FUNC_BODY(elosBlacklistInitialize, safuResultE_t, elosEventFilter_t *blacklist,
               samconfConfig_t const *const config) {
    if (MOCK_IS_ACTIVE(elosBlacklistInitialize)) {
        check_expected_ptr(blacklist);
        check_expected_ptr(config);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosBlacklistInitialize)(blacklist, config);
}

MOCK_FUNC_BODY(elosBlacklistDelete, safuResultE_t, elosEventFilter_t *blacklist) {
    if (MOCK_IS_ACTIVE(elosBlacklistDelete)) {
        check_expected_ptr(blacklist);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosBlacklistDelete)(blacklist);
}
