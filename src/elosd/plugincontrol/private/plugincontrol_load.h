// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/types.h"

#define ELOS_FUNCTABLEENTRIES 4

typedef struct elosFuncTableEntry {
    char const *name;
    void **func;
} elosFuncTableEntry_t;

safuResultE_t elosPluginControlLoadHelperSearchFile(elosPluginControl_t const *control, char **file);
safuResultE_t elosPluginControlLoadHelperGetFunctions(elosPluginControl_t *control);
