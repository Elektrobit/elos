// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/clientauthorization_types.h>
#include <stdbool.h>

__BEGIN_DECLS

/********************************************************************
 * A placeholder implementation for client authorization initialization.
 * Currently does nothing aside from preventing a NULL function pointer.
 *
 * Parameters:
 *     clientAuth : clientAuthorization instance
 *
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 ********************************************************************/
safuResultE_t elosUnixClientAuthorizationInitialize(elosClientAuthorization_t *clientAuth);

/********************************************************************
 * Determines if connection is authorized.
 *
 * Parameters:
 *     clientAuth : clientAuthorization instance
 *     fd         : file descriptor of the incoming connection to verify
 *
 * Returns:
 *     - true if the connection is authorized
 *     - false otherwise
 ********************************************************************/
bool elosUnixClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *const clientAuth, int fd);

/********************************************************************
 * Freeing all ressources.
 *
 * Parameters:
 *     clientAuth : clientAuthorization instance
 *
 * Returns:
 *      - SAFU_RESULT_FAILED on error
 *      - SAFU_RESULT_OK on success
 * ********************************************************************/
safuResultE_t elosUnixClientAuthorizationDelete(elosClientAuthorization_t *clientAuth);

__END_DECLS
