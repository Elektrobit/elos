// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/plugincontrol.h"
#include "safu/vector.h"

safuResultE_t elosPluginControlVectorInitialize(elosPluginControlVector_t *controlVector, size_t elements);

safuResultE_t elosPluginControlVectorPush(elosPluginControlVector_t *controlVector, elosPluginControl_t *control);

safuResultE_t elosPluginControlVectorIterate(elosPluginControlVector_t *controlVector, safuVecFunc_t *func, void *data,
                                             int *iterResult);

safuResultE_t elosPluginControlVectorGetById(elosPluginControlVector_t *controlVector, elosPluginId_t id,
                                             elosPluginControl_t **control);

safuResultE_t elosPluginControlVectorRemoveById(elosPluginControlVector_t *controlVector, elosPluginId_t id);

safuResultE_t elosPluginControlVectorDeleteMembers(elosPluginControlVector_t *controlVector);
