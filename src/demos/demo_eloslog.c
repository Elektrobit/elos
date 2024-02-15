// SPDX-License-Identifier: MIT

#include "elos/event/event_types.h"
#include "elos/libelos/libeloslog.h"

int main() {
    elosSession_t *session;
    const char *path = "/tmp/elos_unix_socket";
    safuResultE_t result = SAFU_RESULT_FAILED;

    /* for tcp/ip uncomment below line and comment
     * the next line
     * result = elosConnectTcpip("127.0.0.1", 54321, &session);
     */

    result = elosConnectUnix(path, &session);

    if (result == SAFU_RESULT_OK) {
        result = elosInitLogger(session);
    }

    elosLog(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, "elos log debug message");
    elosLog(ELOS_MSG_CODE_READ_ERROR, ELOS_SEVERITY_ERROR, ELOS_CLASSIFICATION_PROCESS, "elos log error message");
    elosLog(ELOS_MSG_CODE_INFO_LOG, ELOS_SEVERITY_INFO, ELOS_CLASSIFICATION_PROCESS, "elos log Info message");

    elosDisconnect(session);
    elosDeleteLogger();
    return 0;
}
