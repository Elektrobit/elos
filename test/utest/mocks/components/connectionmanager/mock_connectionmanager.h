// SPDX-License-Identifier: MIT

#ifndef __MOCK_ELOS_CONNECTION_MANAGER_H__
#define __MOCK_ELOS_CONNECTION_MANAGER_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_ELOS_CONNECTION_MANAGER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "connectionmanager/clientblacklist.h"
#include "connectionmanager/connectionmanager.h"

MOCK_FUNC_PROTOTYPE(elosConnectionManagerGetStatus, safuResultE_t, elosConnectionManager_t *context, uint32_t *status)
MOCK_FUNC_PROTOTYPE(elosConnectionManagerThreadGetFreeConnectionSlot, safuResultE_t, elosConnectionManager_t *ctx,
                    int *slot)
MOCK_FUNC_PROTOTYPE(elosConnectionManagerThreadWaitForIncomingConnection, safuResultE_t, elosConnectionManager_t *ctx,
                    int slot)
MOCK_FUNC_PROTOTYPE(elosBlacklistInitialize, safuResultE_t, elosEventFilter_t *blacklist,
                    samconfConfig_t const *const config)
MOCK_FUNC_PROTOTYPE(elosBlacklistDelete, safuResultE_t, elosEventFilter_t *blacklist)

#endif /* __MOCK_ELOS_CONNECTION_MANAGER_H__ */
