// SPDX-License-Identifier: MIT

#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#include "tcp_clientconnection_private.h"

safuResultE_t elosTcpClientConnectionInitialize(elosClientConnection_t *connection) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    struct sockaddr_in *addr = safuAllocMem(NULL, sizeof(struct sockaddr_in));
    if (addr != NULL) {
        connection->clientConnectionContext = addr;
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosTcpClientConnectionDelete(elosClientConnection_t *connection) {
    free(connection->clientConnectionContext);

    return SAFU_RESULT_OK;
}

safuResultE_t elosTcpClientConnectionClose(elosClientConnection_t *connection) {
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