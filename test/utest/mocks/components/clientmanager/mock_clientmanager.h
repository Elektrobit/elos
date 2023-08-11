// SPDX-License-Identifier: MIT

#ifndef __MOCK_ELOS_CLIENT_MANAGER_H__
#define __MOCK_ELOS_CLIENT_MANAGER_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_ELOS_CLIENT_MANAGER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/clientmanager/clientblacklist.h"
#include "elos/clientmanager/clientmanager.h"

MOCK_FUNC_PROTOTYPE(elosClientManagerGetStatus, safuResultE_t, elosClientManagerContext_t *context, uint32_t *status)
MOCK_FUNC_PROTOTYPE(elosClientManagerThreadGetFreeConnectionSlot, safuResultE_t, elosClientManagerContext_t *ctx,
                    int *slot)
MOCK_FUNC_PROTOTYPE(elosClientManagerThreadWaitForIncomingConnection, safuResultE_t, elosClientManagerContext_t *ctx,
                    int slot)
MOCK_FUNC_PROTOTYPE(elosBlacklistInitialize, safuResultE_t, elosEventFilter_t *blacklist,
                    samconfConfig_t const *const config)
MOCK_FUNC_PROTOTYPE(elosBlacklistDelete, safuResultE_t, elosEventFilter_t *blacklist)

#endif /* __MOCK_ELOS_CLIENT_MANAGER_H__ */
