// SPDX-License-Identifier: MIT

#include "dltLoggerBackend.h"

#include <safu/common.h>
#include <safu/defines.h>
#include <safu/log.h>
#include <safu/mutex.h>
#include <safu/result.h>
#include <stdlib.h>

#include "elos/libelosdlt/libelosdlt.h"
#include "elos/libelosdlt/types.h"

safuResultE_t elosDltLoggerBackendNew(elosDltLoggerBackend_t **dlt, elosDltLoggerBackendParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosDltLoggerBackend_t *newBackend = safuAllocMem(NULL, sizeof(elosDltLoggerBackend_t));
    if (newBackend == NULL) {
        safuLogErr("safuAllocMem failed");
        result = SAFU_RESULT_FAILED;
    } else {
        result = elosDltLoggerBackendInit(newBackend, param);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Failed 'elosDltLoggerBackendInit'");
        } else {
            *dlt = newBackend;
        }
    }

    return result;
}

safuResultE_t elosDltLoggerBackendInit(elosDltLoggerBackend_t *dlt, elosDltLoggerBackendParam_t const *param) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dlt == NULL || param == NULL) {
        safuLogErr("Null parameter given");
    } else {
        memset(&dlt->dlt, 0, sizeof(dlt->dlt));

        SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(&dlt->mutex, NULL, result)
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Failed to intialize mutex");
        } else {
            dlt->dlt.pipePath = strdup(param->connectionString == NULL ? "/tmp/dlt" : param->connectionString);
            if (dlt->dlt.pipePath == NULL) {
                safuLogErr("strdup failed");
            } else {
                memcpy(dlt->dlt.ecuId, param->ecuId, ARRAY_SIZE(dlt->dlt.ecuId));
                memcpy(dlt->dlt.appId, param->appId, ARRAY_SIZE(dlt->dlt.appId));
                result = SAFU_RESULT_OK;
            }
        }
    }

    return result;
}

safuResultE_t elosDltLoggerBackendDelete(elosDltLoggerBackend_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dlt == NULL) {
        safuLogErr("Null parameter given");
    } else {
        elosDltLoggerBackendDeleteMembers(dlt);
        free(dlt);
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosDltLoggerBackendDeleteMembers(elosDltLoggerBackend_t *dlt) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (dlt == NULL) {
        safuLogErr("Null parameter given");
    } else {
        SAFU_PTHREAD_MUTEX_DESTROY(&dlt->mutex, result = SAFU_RESULT_FAILED);
        free(dlt->dlt.pipePath);
        memset(&dlt->dlt, 0, sizeof(dlt->dlt));
    }

    return result;
}

safuResultE_t elosDltLoggerBackendStart(elosDltLoggerBackend_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dlt == NULL) {
        safuLogErr("Invalid parameter");
    } else {
        result = elosDltConnectPipe(&dlt->dlt);
        if (result != SAFU_RESULT_OK) {
            safuLogErrF("dlt connect failed on %s", dlt->dlt.pipePath);
        } else {
            result = elosDltRegisterContext(&dlt->dlt);
        }
    }

    return result;
}

safuResultE_t elosDltLoggerBackendStop(elosDltLoggerBackend_t *dlt) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (dlt == NULL) {
        safuLogErr("Null parameter given");
    } else {
        result = elosDltUnregisterContext(&dlt->dlt);
        if (result != SAFU_RESULT_OK) {
            safuLogErrErrno("Unregister dlt context failed");
        }
        elosDltDisconnect(&dlt->dlt);
    }

    return result;
}

safuResultE_t elosDltLoggerBackendPersist(elosDltLoggerBackend_t *dlt, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((dlt == NULL) || (event == NULL)) {
        safuLogErr("Null parameter given");
    } else {
        char *eventStr = NULL;

        result = elosEventSerialize(&eventStr, event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Event serialization failed");
        } else {
            safuLogDebugF("persisting Event: %s", eventStr);

            SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(&dlt->mutex, result)
            elosDltSendUserLog(&dlt->dlt, eventStr);
            SAFU_PTHREAD_MUTEX_UNLOCK(&dlt->mutex, result = SAFU_RESULT_FAILED);

            free(eventStr);
        }
    }

    return result;
}
