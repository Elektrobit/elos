// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_ELOS_CLIENT_MANAGER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/clientmanager/clientmanager.h"

MOCK_FUNC_PROTOTYPE(elosClientManagerInitialize, safuResultE_t, elosClientManager_t *clientmanager,
                    elosClientManagerParam_t const *param)
MOCK_FUNC_PROTOTYPE(elosClientManagerStart, safuResultE_t, elosClientManager_t *clientmanager)
MOCK_FUNC_PROTOTYPE(elosClientManagerStop, safuResultE_t, elosClientManager_t *clientmanager)
MOCK_FUNC_PROTOTYPE(elosClientManagerDeleteMembers, safuResultE_t, elosClientManager_t *clientmanager)
