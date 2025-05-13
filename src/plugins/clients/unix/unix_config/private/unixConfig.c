// SPDX-License-Identifier: MIT

#include <arpa/inet.h>
#include <elos/libelosplugin/libelosPluginConfig.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <libgen.h>
#include <limits.h>
#include <pwd.h>
#include <safu/log.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "unix_config/config.h"

const char *elosUnixConfigGetPath(elosPlugin_t const *plugin) {
    return elosPluginConfigGetString(plugin, "/Config/path", "ELOSD_SOCKET_PATH", ELOSD_SOCKET_PATH);
}

int elosUnixConfigGetConnectionLimit(elosPlugin_t const *plugin) {
    return elosPluginConfigGetInt(plugin, "/Config/ConnectionLimit", "ELOSD_CONNECTION_LIMIT", ELOSD_CONNECTION_LIMIT);
}

static inline safuResultE_t _mkdir(const char *path, mode_t mode) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal = 0;
    char slash[] = "/";
    char currentPath[PATH_MAX] = {0};
    char pathBuffer[PATH_MAX] = {0};
    struct stat statbuf;

    strncpy(pathBuffer, path, sizeof(pathBuffer) - 1);
    char *dirPath = dirname(pathBuffer);
    safuLogDebugF("Create directory: %s", dirPath);

    char *token = strtok(dirPath, slash);
    while (token != NULL) {
        strcat(currentPath, slash);
        strcat(currentPath, token);
        retVal = mkdir(currentPath, mode);
        if (retVal != 0) {
            if (errno != EEXIST) {
                safuLogErrErrnoValue("mkdir failed", retVal);
                result = SAFU_RESULT_FAILED;
                break;
            }
        }
        token = strtok(NULL, slash);
    }

    retVal = stat(currentPath, &statbuf);
    if (retVal != 0 && !S_ISDIR(statbuf.st_mode)) {
        safuLogErrErrnoValue("stat failed", retVal);
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

safuResultE_t elosUnixConfigGetSocketAddress(elosPlugin_t const *plugin, struct sockaddr_un *addr) {
    const char *path = elosUnixConfigGetPath(plugin);
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (access(path, F_OK) == 0) {
        safuLogWarn("Given socket Path exists, unlinking");
        int retVal = unlink(path);
        if (retVal != 0) {
            safuLogErrErrnoValue("unlink socket path failed", retVal);
        } else {
            result = SAFU_RESULT_OK;
        }
    } else if (errno == ENOENT) {
        result = _mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("Create directory for socket path '%s' failed", path);
        }
    } else {
        safuLogErrErrnoValue("access check failed", errno);
    }

    if (result == SAFU_RESULT_OK) {
        strncpy(addr->sun_path, path, sizeof(addr->sun_path) - 1);
        addr->sun_family = AF_UNIX;
    }

    return result;
}
