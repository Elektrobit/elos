// SPDX-License-Identifier: MIT

#include "mock_clientauthorization.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosTcpClientAuthorizationInitialize, safuResultE_t, elosClientAuthorization_t *clientAuth) {
    if (MOCK_IS_ACTIVE(elosTcpClientAuthorizationInitialize)) {
        check_expected_ptr(clientAuth);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosTcpClientAuthorizationInitialize)(clientAuth);
}

MOCK_FUNC_BODY(elosTcpClientAuthorizationDelete, safuResultE_t, elosClientAuthorization_t *clientAuth) {
    if (MOCK_IS_ACTIVE(elosTcpClientAuthorizationDelete)) {
        check_expected_ptr(clientAuth);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosTcpClientAuthorizationDelete)(clientAuth);
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
