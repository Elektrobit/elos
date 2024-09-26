// SPDX-License-Identifier: MIT
#pragma once

#include <elos/event/event.h>
#include <elos/event/event_vector.h>
#include <elos/event/eventqueue_types.h>
#include <elos/libelos/libelos.h>
#include <safu/log.h>
#include <safu/result.h>

namespace elos {

class Elos {
   public:
    safuResultE_t connect(const char *host, uint16_t port, elosSession_t **session);
    safuResultE_t disconnect(elosSession_t **session);
    safuResultE_t publish(elosSession_t **session, const elosEvent_t *event);
    safuResultE_t subscribe(elosSession_t **session, char const **filterRuleArray, size_t filterRuleArraySize,
                            elosEventQueueId_t *eventQueueId);
};
}  // namespace elos
