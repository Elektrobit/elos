// SPDX-License-Identifier: MIT
#include "elos/libelos-cpp/libelos-cpp.h"

#include <cstdlib>
#include <new>

namespace elos {
safuResultE_t Elos::connect(UNUSED const char *host, UNUSED uint16_t port, UNUSED elosSession_t **session) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Connect to Tcpip");
    return result;
}

safuResultE_t Elos::disconnect(UNUSED elosSession_t **session) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Disonnect");
    return result;
}

safuResultE_t Elos::publish(UNUSED elosSession_t **session, UNUSED const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Publish");
    return result;
}

safuResultE_t Elos::subscribe(UNUSED elosSession_t **session, UNUSED const char **filterRuleArray,
                              UNUSED size_t filterRuleArraySize, UNUSED elosEventQueueId_t *eventQueueId) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Subscribe");
    return result;
}
}  // namespace elos
