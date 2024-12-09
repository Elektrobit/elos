// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>

#ifdef MOCK_PLUGIN_CONTROL_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/plugincontrol/plugincontrol.h"

MOCK_FUNC_PROTOTYPE(elosPluginControlInitialize, safuResultE_t, elosPluginControl_t *control,
                    elosPluginControlParam_t const *param)

MOCK_FUNC_PROTOTYPE(elosPluginControlNew, safuResultE_t, elosPluginControl_t **control,
                    elosPluginControlParam_t const *param)

MOCK_FUNC_PROTOTYPE(elosPluginControlLoad, safuResultE_t, elosPluginControl_t *control)

MOCK_FUNC_PROTOTYPE(elosPluginControlUnload, safuResultE_t, elosPluginControl_t *control)

MOCK_FUNC_PROTOTYPE(elosPluginControlStart, safuResultE_t, elosPluginControl_t *control)

MOCK_FUNC_PROTOTYPE(elosPluginControlStop, safuResultE_t, elosPluginControl_t *control)

MOCK_FUNC_PROTOTYPE(elosPluginControlDeleteMembers, safuResultE_t, elosPluginControl_t *control)

MOCK_FUNC_PROTOTYPE(elosPluginControlDelete, safuResultE_t, elosPluginControl_t **control)

MOCK_FUNC_PROTOTYPE(elosPluginControlGetName, safuResultE_t, const elosPluginControl_t *control, const char **name)
