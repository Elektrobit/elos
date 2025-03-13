// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

typedef struct elosPluginControl elosPluginControl_t;
typedef struct elosPluginControlParam elosPluginControlParam_t;
struct elosPublisher;

safuResultE_t elosPluginControlInitialize(elosPluginControl_t *control, elosPluginControlParam_t const *param);
safuResultE_t elosPluginControlNew(elosPluginControl_t **control, elosPluginControlParam_t const *param);
safuResultE_t elosPluginControlLoad(elosPluginControl_t *control);
safuResultE_t elosPluginControlUnload(elosPluginControl_t *control);
safuResultE_t elosPluginControlStart(elosPluginControl_t *control);
safuResultE_t elosPluginControlStop(elosPluginControl_t *control);
safuResultE_t elosPluginControlDeleteMembers(elosPluginControl_t *control);
safuResultE_t elosPluginControlDelete(elosPluginControl_t **control);
safuResultE_t elosPluginControlGetName(const elosPluginControl_t *control, const char **name);
safuResultE_t elosPluginControlCreatePublisher(struct elosPluginControl *pluginControl,
                                               struct elosPublisher **publisher);
safuResultE_t elosPluginControlDeletePublisher(struct elosPluginControl *pluginControl,
                                               struct elosPublisher *publisher);
