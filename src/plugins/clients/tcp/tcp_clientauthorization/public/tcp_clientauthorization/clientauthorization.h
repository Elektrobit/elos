// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>

#include "tcp_clientauthorization/clientauthorization_types.h"

__BEGIN_DECLS

bool elosTcpClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *const clientAuth,
                                                   struct sockaddr const *const addr);
safuResultE_t elosTcpClientAuthorizationInitialize(elosClientAuthorization_t *clientAuth);
safuResultE_t elosTcpClientAuthorizationDelete(elosClientAuthorization_t *clientAuth);

__END_DECLS
