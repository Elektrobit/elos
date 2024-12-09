// SPDX-License-Identifier: MIT

#include "mock_clientmanager.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosClientManagerInitialize, safuResultE_t, elosClientManager_t *clientmanager,
               elosClientManagerParam_t const *param) {
    if (MOCK_IS_ACTIVE(elosClientManagerInitialize)) {
        check_expected_ptr(clientmanager);
        check_expected_ptr(param);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientManagerInitialize)(clientmanager, param);
}

MOCK_FUNC_BODY(elosClientManagerStart, safuResultE_t, elosClientManager_t *clientmanager) {
    if (MOCK_IS_ACTIVE(elosClientManagerStart)) {
        check_expected_ptr(clientmanager);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientManagerStart)(clientmanager);
}

MOCK_FUNC_BODY(elosClientManagerStop, safuResultE_t, elosClientManager_t *clientmanager) {
    if (MOCK_IS_ACTIVE(elosClientManagerStop)) {
        check_expected_ptr(clientmanager);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientManagerStop)(clientmanager);
}

MOCK_FUNC_BODY(elosClientManagerDeleteMembers, safuResultE_t, elosClientManager_t *clientmanager) {
    if (MOCK_IS_ACTIVE(elosClientManagerDeleteMembers)) {
        check_expected_ptr(clientmanager);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientManagerDeleteMembers)(clientmanager);
}
