// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_ELOS_LIBELOSPLUGIN_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include <elos/libelosplugin/clientauthorizedprocesses.h>

MOCK_FUNC_PROTOTYPE(elosAuthorizedProcessInitialize, safuResultE_t, safuVec_t *authorizedprocesses,
                    const samconfConfig_t *config)
MOCK_FUNC_PROTOTYPE(elosAuthorizedProcessDelete, safuResultE_t, safuVec_t *authorizedprocesses)
MOCK_FUNC_PROTOTYPE(elosAuthorizedProcessCheck, bool, safuVec_t *authorizedprocesses, elosProcessIdentity_t *process)
