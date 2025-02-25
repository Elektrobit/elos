// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/clientauthorization_types.h>
#include <netinet/in.h>
#include <stdbool.h>

typedef enum elosTcpStatusE {
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING
} elosTcpStatusE_t;

__BEGIN_DECLS

bool elosTcpClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *const clientAuth,
                                                   struct sockaddr const *const addr);
safuResultE_t elosTcpClientAuthorizationInitialize(elosClientAuthorization_t *clientAuth);
safuResultE_t elosTcpClientAuthorizationDelete(elosClientAuthorization_t *clientAuth);

__END_DECLS
