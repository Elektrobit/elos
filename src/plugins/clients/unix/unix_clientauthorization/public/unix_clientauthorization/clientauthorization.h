// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/clientauthorization_types.h>
#include <netinet/in.h>
#include <stdbool.h>

__BEGIN_DECLS

bool elosUnixClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *const clientAuth,
                                                    struct sockaddr const *const addr);
safuResultE_t elosUnixClientAuthorizationInitialize(elosClientAuthorization_t *clientAuth);
safuResultE_t elosUnixClientAuthorizationDelete(elosClientAuthorization_t *clientAuth);

__END_DECLS
