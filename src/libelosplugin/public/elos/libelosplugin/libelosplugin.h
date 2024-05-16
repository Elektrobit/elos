// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosplugin/types.h"

__BEGIN_DECLS

safuResultE_t elosPluginReportAsStarted(elosPlugin_t *plugin);
safuResultE_t elosPluginStopTriggerWait(elosPlugin_t *plugin);
safuResultE_t elosPluginStopTriggerWrite(elosPlugin_t *plugin);
const char *elosPluginTypeToStr(elosPluginTypeE_t type);

__END_DECLS
