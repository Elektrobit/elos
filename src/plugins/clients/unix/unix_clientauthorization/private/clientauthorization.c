// SPDX-License-Identifier: MIT
#define _GNU_SOURCE
#include "unix_clientauthorization/clientauthorization.h"

#include <elos/libelosplugin/clientauthorizedprocesses.h>
#include <limits.h>
#include <safu/log.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

safuResultE_t elosUnixClientAuthorizationInitialize(elosClientAuthorization_t *const clientAuth) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (clientAuth == NULL) {
        safuLogErr("Invalid argument");
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

static inline safuResultE_t _getExecPath(char const *const line, char **execPath) {
    safuResultE_t result = SAFU_RESULT_OK;
    char prog[PATH_MAX] = {0};

    ssize_t length = readlink(line, prog, PATH_MAX - 1);
    if (length == -1) {
        result = SAFU_RESULT_FAILED;
    } else {
        prog[length] = '\0';
        *execPath = strdup(prog);
        if (*execPath == NULL) {
            safuLogErr("Failed to allocate string");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

bool elosUnixClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *const clientAuth, int fd) {
    struct ucred clientCredentials;
    socklen_t length = sizeof(clientCredentials);
    bool isClientAuthorized = false;
    char line[ELOS_LINE_MAX] = {0};
    elosProcessIdentity_t process = {0};

    safuLogDebug("Check for trusted connection");

    int retVal = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &clientCredentials, &length);
    if (retVal == -1) {
        safuLogErrErrnoValue("Get socket options failed", retVal);
    } else {
        process.pid = clientCredentials.pid;
        process.gid = clientCredentials.gid;
        process.uid = clientCredentials.uid;

        snprintf(line, ELOS_LINE_MAX, "%s/%d/%s", PATH_PROC, process.pid, PATH_EXELINE);

        if (_getExecPath(line, &process.exec) == SAFU_RESULT_OK) {
            isClientAuthorized = elosAuthorizedProcessCheck(&clientAuth->authorizedProcessFilters, &process);
            free(process.exec);
        }
    }

    return isClientAuthorized;
}

safuResultE_t elosUnixClientAuthorizationDelete(elosClientAuthorization_t *const clientAuth) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (clientAuth == NULL) {
        safuLogErr("Invalid argument");
        result = SAFU_RESULT_FAILED;
    } else {
        if (clientAuth->authorizedProcessFilters.elementSize > 0) {
            result = elosAuthorizedProcessDelete(&clientAuth->authorizedProcessFilters);
        }
    }

    return result;
}
