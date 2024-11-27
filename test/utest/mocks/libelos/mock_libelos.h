// SPDX-License-Identifier: MIT
#ifndef ELOS_LIBELOS_LIBELOS_MOCK_H
#define ELOS_LIBELOS_LIBELOS_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_LIBELOS_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/libelos/libelos.h"

MOCK_FUNC_PROTOTYPE(elosConnectTcpip, safuResultE_t, const char *ip, uint16_t port, elosSession_t **session)

MOCK_FUNC_PROTOTYPE(elosDisconnect, safuResultE_t, elosSession_t *session)

MOCK_FUNC_PROTOTYPE(elosConnectSessionTcpip, safuResultE_t, const char *host, uint16_t port, elosSession_t *session)

MOCK_FUNC_PROTOTYPE(elosDisconnectSession, safuResultE_t, elosSession_t *session)

MOCK_FUNC_PROTOTYPE(elosGetVersion, safuResultE_t, elosSession_t *session, const char **version)

MOCK_FUNC_PROTOTYPE(elosLogFindEvent, safuResultE_t, elosSession_t *session, const char *filterRule,
                    elosEventVector_t **vector)

MOCK_FUNC_PROTOTYPE(elosFindEvents, safuResultE_t, elosSession_t *session, const char *filterRule,
                    struct timespec const *newest, struct timespec const *oldest, elosEventVector_t **vector)

MOCK_FUNC_PROTOTYPE(elosSessionValid, bool, elosSession_t const *session)

MOCK_FUNC_PROTOTYPE(elosEventSubscribe, safuResultE_t, elosSession_t *session, char const **filterRuleArray,
                    size_t filterRuleArraySize, elosEventQueueId_t *eventQueueId)

MOCK_FUNC_PROTOTYPE(elosEventUnsubscribe, safuResultE_t, elosSession_t *session, elosEventQueueId_t eventQueueId)

MOCK_FUNC_PROTOTYPE(elosEventPublish, safuResultE_t, elosSession_t *session, const elosEvent_t *event)

MOCK_FUNC_PROTOTYPE(elosEventQueueRead, safuResultE_t, elosSession_t *session, elosEventQueueId_t eventQueueId,
                    elosEventVector_t **eventVector)

#endif /* ELOS_LIBELOS_LIBELOS_MOCK_H */
