// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>

#include "elos/storagemanager/vector.h"

safuResultE_t elosStorageBackendPtrVectorInitialize(elosStorageBackendPtrVector_t *backends, size_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backends == NULL) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecCreate(backends, elements, sizeof(elosStorageBackend_t *));
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosStorageBackendPtrVectorPush(elosStorageBackendPtrVector_t *backends, elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backends == NULL) || (backend == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecPush(backends, &backend);
        if (retVal < 0) {
            safuLogErr("safuVecPush failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosStorageBackendPtrVectorIterate(elosStorageBackendPtrVector_t *backends, safuVecFunc_t *func,
                                                 void *data, int *iterResult) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backends == NULL) || (func == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecIterate(backends, func, data);
        if (retVal < 0) {
            safuLogWarn("safuVecIterate failed");
        } else {
            result = SAFU_RESULT_OK;
        }

        if (iterResult != NULL) {
            *iterResult = retVal;
        }
    }

    return result;
}

static int _backendVectorMatchByName(const void *element, const void *data) {
    const elosStorageBackend_t *backend = element;
    const char *const name = data;
    int result = 0;

    if (strcmp(backend->name, name) == 0) {
        result = 1;
    }

    return result;
}

safuResultE_t elosStorageBackendPtrVectorGetByName(elosStorageBackendPtrVector_t *backends, const char *const name,
                                                   elosStorageBackend_t **backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backends == NULL) || safuStringIsEmpty(name) == SAFU_RESULT_OK || (backend == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecFindGet(backends, (void **)backend, NULL, _backendVectorMatchByName, name);
        if (retVal < 0) {
            safuLogErr("safuVecFindGet failed");
        } else if (retVal == 0) {
            result = SAFU_RESULT_NOT_FOUND;
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static int _backendVectorRemoveByName(const void *element, const void *data) {
    elosStorageBackend_t *backend = (elosStorageBackend_t *)element;
    const char *const name = data;
    int result = 0;

    if (strcmp(backend->name, name) == 0) {
        result = 1;
    }

    return result;
}

safuResultE_t elosStorageBackendPtrVectorRemoveByName(elosStorageBackendPtrVector_t *backends, const char *const name) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((backends == NULL) || (safuStringIsEmpty(name) == SAFU_RESULT_OK)) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecFindRemove(backends, _backendVectorRemoveByName, name);
        if (retVal < 0) {
            safuLogErr("safuVecFindRemove failed");
        } else if (retVal == 0) {
            result = SAFU_RESULT_NOT_FOUND;
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosStorageBackendPtrVectorDeleteMembers(elosStorageBackendPtrVector_t *backends) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backends == NULL) {
        safuLogErr("Null parameter given");
    } else {
        int retVal;

        retVal = safuVecFree(backends);
        if (retVal < 0) {
            safuLogErr("safuVecCreate failed");
        } else {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
