// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#ifdef EVENTDISPATCHER_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/eventdispatcher/eventdispatcher.h"

MOCK_FUNC_PROTOTYPE(elosEventDispatcherInitialize, safuResultE_t, elosEventDispatcher_t *eventDispatcher,
                    elosEventDispatcherParam_t const *const param)

MOCK_FUNC_PROTOTYPE(elosEventDispatcherNew, safuResultE_t, elosEventDispatcher_t **eventDispatcher,
                    elosEventDispatcherParam_t const *const param)

MOCK_FUNC_PROTOTYPE(elosEventDispatcherDeleteMembers, safuResultE_t, elosEventDispatcher_t *eventDispatcher)

MOCK_FUNC_PROTOTYPE(elosEventDispatcherDelete, safuResultE_t, elosEventDispatcher_t **eventDispatcher)

MOCK_FUNC_PROTOTYPE(elosEventDispatcherStart, safuResultE_t, elosEventDispatcher_t *eventDispatcher)

MOCK_FUNC_PROTOTYPE(elosEventDispatcherStop, safuResultE_t, elosEventDispatcher_t *eventDispatcher)
