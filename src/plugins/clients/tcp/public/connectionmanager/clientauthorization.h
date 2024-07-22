// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>

#include "connectionmanager/clientauthorization_types.h"

__BEGIN_DECLS

bool elosClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *clientAuth, struct sockaddr_in const *addr);
safuResultE_t elosClientAuthorizationInitialize(elosClientAuthorization_t *clientAuth);
safuResultE_t elosClientAuthorizationDelete(elosClientAuthorization_t *clientAuth);

__END_DECLS
