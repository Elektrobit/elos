// SPDX-License-Identifier: MIT

#include "mock_client_authorized_processes.h"

#include <cmocka_extensions/cmocka_extensions.h>

MOCK_FUNC_BODY(elosAuthorizedProcessInitialize, safuResultE_t, safuVec_t *authorizedprocesses,
               const samconfConfig_t *config) {
    if (MOCK_IS_ACTIVE(elosAuthorizedProcessInitialize)) {
        check_expected_ptr(authorizedprocesses);
        check_expected_ptr(config);
        authorizedprocesses = mock_ptr_type(struct safuVec *);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosAuthorizedProcessInitialize)(authorizedprocesses, config);
}

MOCK_FUNC_BODY(elosAuthorizedProcessDelete, safuResultE_t, safuVec_t *authorizedprocesses) {
    if (MOCK_IS_ACTIVE(elosAuthorizedProcessDelete)) {
        check_expected_ptr(authorizedprocesses);
        return mock_type(safuResultE_t);
    }
    return MOCK_FUNC_REAL(elosAuthorizedProcessDelete)(authorizedprocesses);
}

MOCK_FUNC_BODY(elosAuthorizedProcessCheck, bool, safuVec_t *authorizedprocesses, elosProcessIdentity_t *process) {
    if (MOCK_IS_ACTIVE(elosAuthorizedProcessCheck)) {
        check_expected_ptr(authorizedprocesses);
        check_expected_ptr(process);
        return mock_type(bool);
    }
    return MOCK_FUNC_REAL(elosAuthorizedProcessCheck)(authorizedprocesses, process);
}
