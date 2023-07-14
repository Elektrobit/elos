// SPDX-License-Identifier: MIT
#ifndef __ELOS_EVENTLOGGING_STORAGEBACKEND_H__
#define __ELOS_EVENTLOGGING_STORAGEBACKEND_H__

#include <safu/common.h>
#include <safu/vector.h>

#include "elos/event/event.h"
#include "elos/rpnfilter/rpnfilter.h"

struct elosStorageBackend;
typedef struct elosStorageBackend elosStorageBackend_t;

typedef safuResultE_t elosStorageBackendStart_t(elosStorageBackend_t *backend);
typedef safuResultE_t elosStorageBackendPersist_t(elosStorageBackend_t *backend, const elosEvent_t *event);
typedef safuResultE_t elosStorageBackendFindEvent_t(elosStorageBackend_t *backend, elosRpnFilter_t *filter,
                                                    safuVec_t *events);
typedef safuResultE_t elosStorageBackendShutdown_t(elosStorageBackend_t *backend);

safuResultE_t elosStorageBackendAccepts(const elosStorageBackend_t *backend, const elosEvent_t *event);

struct elosStorageBackend {
    const char *name;
    void *backendData;
    safuVec_t filter;

    elosStorageBackendStart_t *start;
    elosStorageBackendPersist_t *persist;
    elosStorageBackendFindEvent_t *findEvent;
    elosStorageBackendShutdown_t *shutdown;
};

#endif /* __ELOS_EVENTLOGGING_STORAGEBACKEND_H__ */
