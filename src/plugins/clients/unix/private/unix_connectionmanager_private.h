// SPDX-License-Identifier: MIT

#pragma once

#include <elos/libelosplugin/connectionmanager.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unix_clientauthorization/clientauthorization.h>
#include <unix_config/config.h>

safuResultE_t elosUnixConnectionManagerNew(elosConnectionManager_t **connectionManager, elosPlugin_t *plugin);
