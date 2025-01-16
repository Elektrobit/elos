// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>

#include "tcp_clientauthorization/clientauthorization_types.h"

__BEGIN_DECLS

bool elosTcpClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *const clientAuth,
                                                   struct sockaddr const *const addr);
safuResultE_t elosClientAuthorizationInitialize(elosClientAuthorization_t *clientAuth);
safuResultE_t elosClientAuthorizationDelete(elosClientAuthorization_t *clientAuth);

__END_DECLS
