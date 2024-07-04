// SPDX-License-Identifier: MIT
#pragma once

#include "elos/libelosplugin/StorageBackend_types.h"

__BEGIN_DECLS

/**
 * Determine if a Storage Backend is configured to accept and process a given event.
 *
 * Parameters:
 *     backend (elosStorageBackend_t *): The backend that should be checked.
 *     event (elosEvent_t *) : The event in question.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – The event shall be processed by the backend.
 *         SAFU_RESULT_NOT_FOUND – The event shall **not** be processed by the
 *         backend.
 *         SAFU_RESULT_FAILURE – The event could not be checked due to some
 *         internal error. Check to  error logs for details.
 **/
safuResultE_t elosStorageBackendAccepts(const elosStorageBackend_t *backend, const elosEvent_t *event);

__END_DECLS
