// SPDX-License-Identifier: MIT
#pragma once

#include "mock_libelosplugin.h"
#include "plugincontrol_stubs.h"

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

MOCK_FUNC_PROTOTYPE(elosPluginControlCreatePublisher, safuResultE_t, struct elosPluginControl *pluginControl,
                    struct elosPublisher **publisher)

MOCK_FUNC_PROTOTYPE(elosPluginControlDeletePublisher, safuResultE_t, struct elosPluginControl *pluginControl,
                    struct elosPublisher *publisher)