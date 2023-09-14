// SPDX-License-Identifier: MIT

#include "elos/event/event_types.h"
#include "elos/libelos/libeloslog.h"

int main() {
    elosLog(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, "elos log debug message");
    elosLog(ELOS_MSG_CODE_READ_ERROR, ELOS_SEVERITY_ERROR, ELOS_CLASSIFICATION_PROCESS, "elos log error message");
    elosLog(ELOS_MSG_CODE_INFO_LOG, ELOS_SEVERITY_INFO, ELOS_CLASSIFICATION_PROCESS, "elos log Info message");
    return 0;
}
