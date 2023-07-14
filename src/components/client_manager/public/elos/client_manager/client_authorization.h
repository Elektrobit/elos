// SPDX-License-Identifier: MIT
#ifndef ELOS_CLIENT_AUTHORIZATION_H_
#define ELOS_CLIENT_AUTHORIZATION_H_

#include <netinet/in.h>

#include "elos/client_manager/client_authorization_types.h"

bool elosClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *clientAuth, struct sockaddr_in const *addr);
safuResultE_t elosClientAuthorizationInitialize(elosClientAuthorization_t *clientAuth);
safuResultE_t elosClientAuthorizationDelete(elosClientAuthorization_t *clientAuth);

#endif
