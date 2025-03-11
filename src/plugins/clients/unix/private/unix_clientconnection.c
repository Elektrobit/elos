// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>

#include "unix_clientconnection_private.h"

safuResultE_t elosUnixClientConnectionInitialize(elosClientConnection_t *connection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    struct sockaddr_un *addr = safuAllocMem(NULL, sizeof(struct sockaddr_un));
    if (addr != NULL) {
        connection->clientConnectionContext = addr;
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosUnixClientConnectionDelete(elosClientConnection_t *connection) {
    free(connection->clientConnectionContext);

    return SAFU_RESULT_OK;
}

safuResultE_t elosUnixClientConnectionClose(elosClientConnection_t *connection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (connection->fd != -1) {
        int retVal = close(connection->fd);
        if (retVal != 0) {
            safuLogWarnErrnoValue("Closing socketFd failed", retVal);
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}