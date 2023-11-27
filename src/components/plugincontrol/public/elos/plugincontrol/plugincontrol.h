// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/defines.h"
#include "elos/plugincontrol/types.h"

safuResultE_t elosPluginControlInitialize(elosPluginControl_t *control, elosPluginControlParam_t const *param);

safuResultE_t elosPluginControlNew(elosPluginControl_t **control, elosPluginControlParam_t const *param);

safuResultE_t elosPluginControlLoad(elosPluginControl_t *control);

safuResultE_t elosPluginControlUnload(elosPluginControl_t *control);

safuResultE_t elosPluginControlStart(elosPluginControl_t *control);

safuResultE_t elosPluginControlStop(elosPluginControl_t *control);

safuResultE_t elosPluginControlDeleteMembers(elosPluginControl_t *control);

safuResultE_t elosPluginControlDelete(elosPluginControl_t **control);

safuResultE_t elosPluginControlGetName(const elosPluginControl_t *control, const char **name);
