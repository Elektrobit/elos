// SPDX-License-Identifier: MIT

#include "mock_client_manager.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosClientManagerGetStatus, safuResultE_t, elosClientManagerContext_t *context, uint32_t *status) {
    if (MOCK_IS_ACTIVE(elosClientManagerGetStatus)) {
        check_expected_ptr(context);
        check_expected_ptr(status);
        *status = mock_type(uint32_t);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientManagerGetStatus)(context, status);
}

MOCK_FUNC_BODY(elosClientManagerThreadGetFreeConnectionSlot, safuResultE_t, elosClientManagerContext_t *ctx,
               int *slot) {
    if (MOCK_IS_ACTIVE(elosClientManagerThreadGetFreeConnectionSlot)) {
        check_expected_ptr(ctx);
        check_expected_ptr(slot);
        *slot = mock_type(int);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientManagerThreadGetFreeConnectionSlot)(ctx, slot);
}

MOCK_FUNC_BODY(elosClientManagerThreadWaitForIncomingConnection, safuResultE_t, elosClientManagerContext_t *ctx,
               int slot) {
    if (MOCK_IS_ACTIVE(elosClientManagerThreadWaitForIncomingConnection)) {
        check_expected_ptr(ctx);
        check_expected(slot);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientManagerThreadWaitForIncomingConnection)(ctx, slot);
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
