// SPDX-License-Identifier: MIT

#include "tcp_config/config.h"

#include <elos/libelosplugin/libelosPluginConfig.h>
#include <elos/libelosplugin/libelosplugin.h>

int elosTcpConfigGetPort(elosPlugin_t const *plugin) {
    return elosPluginConfigGetInt(plugin, "/Config/Port", "ELOSD_PORT", ELOSD_PORT);
}

int elosTcpConfigGetConnectionLimit(elosPlugin_t const *plugin) {
    return elosPluginConfigGetInt(plugin, "/Config/ConnectionLimit", "ELOSD_CONNECTION_LIMIT", ELOSD_CONNECTION_LIMIT);
}

const char *elosTcpConfigGetInterface(elosPlugin_t const *plugin) {
    return elosPluginConfigGetString(plugin, "/Config/Interface", "ELOSD_INTERFACE", ELOSD_INTERFACE);
}
