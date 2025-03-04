// SPDX-License-Identifier: MIT

#pragma once

#include <elos/libelosplugin/connectionmanager.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>
#include <tcp_clientauthorization/clientauthorization.h>
#include <tcp_config/config.h>

#include "tcp_connectionmanager_private.h"

safuResultE_t elosTcpConnectionManagerNew(elosConnectionManager_t **connectionManager, elosPlugin_t *plugin);
