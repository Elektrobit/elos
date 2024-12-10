// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_PLUGIN_CONTROL_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/plugincontrol/vector.h"

MOCK_FUNC_PROTOTYPE(elosPluginControlPtrVectorInitialize, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
                    size_t elements)

MOCK_FUNC_PROTOTYPE(elosPluginControlPtrVectorPush, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
                    elosPluginControl_t *control)

MOCK_FUNC_PROTOTYPE(elosPluginControlPtrVectorIterate, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
                    safuVecFunc_t *func, void *data, int *iterResult)

MOCK_FUNC_PROTOTYPE(elosPluginControlPtrVectorGetById, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
                    elosPluginId_t id, elosPluginControl_t **control)

MOCK_FUNC_PROTOTYPE(elosPluginControlPtrVectorRemoveById, safuResultE_t, elosPluginControlPtrVector_t *controlPtrVector,
                    elosPluginId_t id)

MOCK_FUNC_PROTOTYPE(elosPluginControlPtrVectorDeleteMembers, safuResultE_t,
                    elosPluginControlPtrVector_t *controlPtrVector)
