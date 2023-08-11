// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_ELOS_CLIENT_MANAGER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/clientmanager/clientauthorization.h"

MOCK_FUNC_PROTOTYPE(elosClientAuthorizationInitialize, safuResultE_t, elosClientAuthorization_t *clientAuth)
MOCK_FUNC_PROTOTYPE(elosClientAuthorizationDelete, safuResultE_t, elosClientAuthorization_t *clientAuth)
MOCK_FUNC_PROTOTYPE(elosClientAuthorizationIsTrustedConnection, bool, elosClientAuthorization_t *clientAuth,
                    struct sockaddr_in const *addr)
