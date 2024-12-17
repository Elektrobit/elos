// SPDX-License-Identifier: MIT

#include <arpa/inet.h>
#include <elos/libelosplugin/libelosPluginConfig.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/log.h>

#include "tcp_config/config.h"

int elosTcpConfigGetPort(elosPlugin_t const *plugin) {
    return elosPluginConfigGetInt(plugin, "/Config/Port", "ELOSD_PORT", ELOSD_PORT);
}

int elosTcpConfigGetConnectionLimit(elosPlugin_t const *plugin) {
    return elosPluginConfigGetInt(plugin, "/Config/ConnectionLimit", "ELOSD_CONNECTION_LIMIT", ELOSD_CONNECTION_LIMIT);
}

const char *elosTcpConfigGetInterface(elosPlugin_t const *plugin) {
    return elosPluginConfigGetString(plugin, "/Config/Interface", "ELOSD_INTERFACE", ELOSD_INTERFACE);
}

safuResultE_t elosTcpConfigGetSocketAddress(elosPlugin_t const *plugin, struct sockaddr *addr) {
    safuResultE_t result = SAFU_RESULT_OK;
    struct sockaddr_in *addrIp = (struct sockaddr_in *)addr;
    char const *interface = elosTcpConfigGetInterface(plugin);
    int const port = elosTcpConfigGetPort(plugin);

    memset(addr, 0, sizeof(struct sockaddr_in));

    int retVal = inet_pton(AF_INET, interface, &addrIp->sin_addr);
    if (retVal != 1) {
        safuLogErrErrnoValue("inet_pton failed", retVal);
        result = SAFU_RESULT_FAILED;
    } else {
        addrIp->sin_family = AF_INET;
        addrIp->sin_port = htons(port);
    }

    return result;
}
