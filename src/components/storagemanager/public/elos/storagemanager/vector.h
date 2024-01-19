// SPDX-License-Identifier: MIT
#pragma once

#include <safu/vector.h>

#include "elos/storagemanager/StorageBackend.h"

__BEGIN_DECLS

/*******************************************************************
 * Initialize a new vector with a initial size of `count`. Uses internaly
 * `safuVecCreate`to create the new vector. It is undefined behavior if a
 * vector initialized by this funciton is freed using `safuVecFree`, use
 * `elosStorageBackendPtrVectorDeleteMembers` instead.
 *
 * Parameters:
 *     backends (elosStorageBackendPtrVector_t *): Pointer to store the vector struct.
 *     elements (size_t): Number of elements that the vector should store.
 *
 * Returns:
 *     safuResultE_t:
 *     - SAFU_RESULT_OK if successful
 *     - SAFU_RESULT_FAILED in error case
 ******************************************************************/
safuResultE_t elosStorageBackendPtrVectorInitialize(elosStorageBackendPtrVector_t *backends, size_t elements);

/*******************************************************************
 * Add the address of a storage backend to the end of a the vector using
 * `safuVecPush`.
 *
 * Parameters:
 *     backends (elosStorageBackendPtrVector_t *): Pointer to the vector which should gain a new element.
 *     backend (elosStorageBackend_t*): Pointer to the element that should be added to
 *              the vector.
 * Returns:
 *     safuResultE_t:
 *     - SAFU_RESULT_OK if successful
 *     - SAFU_RESULT_FAILED in error case
 ******************************************************************/
safuResultE_t elosStorageBackendPtrVectorPush(elosStorageBackendPtrVector_t *backends, elosStorageBackend_t *backend);

/*******************************************************************
 * Iterate over the given vector and call the given function for each storage backend.
 *
 * Parameters:
 *     backends (elosStorageBackendPtrVector_t *): Pointer to the vector to iterate.
 *     func (safuVecFunc_t*): Function to call on each element. Must return `0` on each iteration to continue or `1` to
 *                            stop  iteration.
 *     iterResult (int): Result value of the last iteration function call.
 *
 * Returns:
 *     safuResultE_t:
 *     - SAFU_RESULT_OK if iteration finished successful (iterResult >=0)
 *     - SAFU_RESULT_FAILED if iteration failed (iterResult < 0)
 ******************************************************************/
safuResultE_t elosStorageBackendPtrVectorIterate(elosStorageBackendPtrVector_t *backends, safuVecFunc_t *func,
                                                 void *data, int *iterResult);

/*******************************************************************
 * Look for a storage backend by its name. If the vector contains multiple
 * storage backends with the same name only the first can be retrieved by this
 * function.
 *
 * Parameters:
 *     backends (elosStorageBackendPtrVector_t *): Pointer to the vector to by searched.
 *     name (const char *): The name of the storage backend to search for.
 *     backend (elosStorageBackend_t **): Pointer to a backend pointer to contain the first backend with given name.
 *
 * Returns:
 *     safuResultE_t:
 *     - SAFU_RESULT_OK If backend was found and `backend` is set.
 *     - SAFU_RESULT_NOT_FOUND If no backend was found.
 *     - SAFU_RESULT_FAILED If lookup failed for any reason.
 ******************************************************************/
safuResultE_t elosStorageBackendPtrVectorGetByName(elosStorageBackendPtrVector_t *backends, const char *name,
                                                   elosStorageBackend_t **backend);

/*******************************************************************
 * Remove a storage backend pointer with given name from the vector. If the
 * vector contains multiple storage backends with the same name only the first
 * entry is removed.
 *
 * Parameters:
 *     backends (elosStorageBackendPtrVector_t *): Pointer to the vector to remove an element from.
 *     name (const char *): The name of the storage backend to remove.
 *
 * Returns:
 *     safuResultE_t:
 *     - SAFU_RESULT_OK If backend was found and `backend` is set.
 *     - SAFU_RESULT_NOT_FOUND If no backend was found / removed.
 *     - SAFU_RESULT_FAILED If lookup failed for any reason.
 ******************************************************************/
safuResultE_t elosStorageBackendPtrVectorRemoveByName(elosStorageBackendPtrVector_t *backends, const char *name);

/*******************************************************************
 * Free all allocated resource of a `elosStorageBackendPtrVector_t` initialized by
 * `elosStorageBackendPtrVectorInitialize`.
 *
 * Parameters:
 *     backends (elosStorageBackendPtrVector_t *): Pointer to the vector be freed.
 *
 * Returns:
 *     safuResultE_t:
 *     - SAFU_RESULT_OK if successful
 *     - SAFU_RESULT_FAILED in error case
 ******************************************************************/
safuResultE_t elosStorageBackendPtrVectorDeleteMembers(elosStorageBackendPtrVector_t *backends);

__END_DECLS
