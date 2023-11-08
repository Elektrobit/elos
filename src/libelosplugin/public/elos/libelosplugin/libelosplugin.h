// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosplugin/types.h"

safuResultE_t elosPluginReportAsStarted(elosPluginContext_t *plugin);
safuResultE_t elosPluginStopTriggerWait(elosPluginContext_t *plugin);
safuResultE_t elosPluginStopTriggerWrite(elosPluginContext_t *plugin);
