// SPDX-License-Identifier: MIT

#include "mock_clientauthorization.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosClientAuthorizationInitialize, safuResultE_t, elosClientAuthorization_t *clientAuth) {
    if (MOCK_IS_ACTIVE(elosClientAuthorizationInitialize)) {
        check_expected_ptr(clientAuth);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientAuthorizationInitialize)(clientAuth);
}

MOCK_FUNC_BODY(elosClientAuthorizationDelete, safuResultE_t, elosClientAuthorization_t *clientAuth) {
    if (MOCK_IS_ACTIVE(elosClientAuthorizationDelete)) {
        check_expected_ptr(clientAuth);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosClientAuthorizationDelete)(clientAuth);
}

MOCK_FUNC_BODY(elosTcpClientAuthorizationIsTrustedConnection, bool, elosClientAuthorization_t *clientAuth,
               struct sockaddr const *addr) {
    if (MOCK_IS_ACTIVE(elosTcpClientAuthorizationIsTrustedConnection)) {
        check_expected_ptr(clientAuth);
        check_expected_ptr(addr);
        return mock_type(bool);
    }
    return MOCK_FUNC_REAL(elosTcpClientAuthorizationIsTrustedConnection)(clientAuth, addr);
}
