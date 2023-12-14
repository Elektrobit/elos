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

MOCK_FUNC_PROTOTYPE(elosPluginControlVectorInitialize, safuResultE_t, elosPluginControlVector_t *controlVector,
                    size_t elements)

MOCK_FUNC_PROTOTYPE(elosPluginControlVectorPush, safuResultE_t, elosPluginControlVector_t *controlVector,
                    elosPluginControl_t *control)

MOCK_FUNC_PROTOTYPE(elosPluginControlVectorIterate, safuResultE_t, elosPluginControlVector_t *controlVector,
                    safuVecFunc_t *func, void *data, int *iterResult)

MOCK_FUNC_PROTOTYPE(elosPluginControlVectorGetById, safuResultE_t, elosPluginControlVector_t *controlVector,
                    elosPluginId_t id, elosPluginControl_t **control)

MOCK_FUNC_PROTOTYPE(elosPluginControlVectorRemoveById, safuResultE_t, elosPluginControlVector_t *controlVector,
                    elosPluginId_t id)

MOCK_FUNC_PROTOTYPE(elosPluginControlVectorDeleteMembers, safuResultE_t, elosPluginControlVector_t *controlVector)
