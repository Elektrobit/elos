// SPDX-License-Identifier: MIT
#ifndef __ELOS_EVENTLOGGING_STORAGEBACKEND_H__
#define __ELOS_EVENTLOGGING_STORAGEBACKEND_H__

#include <safu/common.h>
#include <safu/vector.h>

#include "elos/event/event.h"
#include "elos/eventfilter/eventfilter_types.h"

struct elosStorageBackend;
typedef struct elosStorageBackend elosStorageBackend_t;

__BEGIN_DECLS

#ifdef SPHINX_C_AUTODOC_USE_BROKEN_FUNC_POINTER_TYPEDEFS
/**
 * A hook to be called by the system to run an instance of the Storage Backend
 * plugin.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
#endif
typedef safuResultE_t elosStorageBackendStart_t(elosStorageBackend_t *backend);

#ifdef SPHINX_C_AUTODOC_USE_BROKEN_FUNC_POINTER_TYPEDEFS
/**
 * A hook to be called by the system to persist an event for the given instance
 * of the Storage Backend plugin.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *     event (elosEvent_t*): The event to be stored.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
#endif
typedef safuResultE_t elosStorageBackendPersist_t(elosStorageBackend_t *backend, const elosEvent_t *event);

#ifdef SPHINX_C_AUTODOC_USE_BROKEN_FUNC_POINTER_TYPEDEFS
/**
 * A hook to be called by the system to find all events in an instance of the
 * Storage Backend plugin that match a given filter.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *     filter (safuVec_t*): The filter to find matching events.
 *     newest (struct timespec const *): the most recent timetamp to check for
 *     oldest (struct timespec const *): timstamp befor oldest event to find
 *     events (safuVec_t*): A vector to store matching events.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – On success, the `events`vector contains all
 *         matching events or is empty if no events could be found.
 *         SAFU_RESULT_FAILURE – on failure. Check the logs for details.
 */
#endif
typedef safuResultE_t elosStorageBackendFindEvent_t(elosStorageBackend_t *backend, elosEventFilter_t *filter,
                                                    struct timespec const *newest, struct timespec const *oldest,
                                                    safuVec_t *events);

#ifdef SPHINX_C_AUTODOC_USE_BROKEN_FUNC_POINTER_TYPEDEFS
/**
 * The shutdown hook to be called by the system to shutdown the given instance
 * of the Storage Backend plugin.
 *
 * Parameters:
 *     backend (elosStorageBackend_t*): The Storage Backend instance itself.
 *
 * Returns:
 *     safuResultE_t:
 *         SAFU_RESULT_OK – on success.
 *         SAFU_RESULT_FAILURE – on failure.
 */
#endif
typedef safuResultE_t elosStorageBackendShutdown_t(elosStorageBackend_t *backend);

/**
 * The StorageBackend represents an instance of a specific Storage Backend.
 * Members:
 *     name: The Storage Backend instance identifier
 *     backendData: A plugin specific context for this instance
 *     filter: Vector of filters to determine which events should accepted and
 *             processed by this instance.
 *     start: Function pointer to plugin specific start function.
 *     persist: Function pointer to plugin specific persist function.
 *     findEvent: Function pointer to plugin specific findEvent function.
 *     shutdown: Function pointer to plugin specific stop function.
 **/
struct elosStorageBackend {
    const char *name;
    void *backendData;
    safuVec_t filter;

    elosStorageBackendStart_t *start;
    elosStorageBackendPersist_t *persist;
    elosStorageBackendFindEvent_t *findEvent;
    elosStorageBackendShutdown_t *shutdown;
};

/**
 * A `safuVect_t` to store `StorageBackend_t*`
 */
typedef safuVec_t elosStorageBackendPtrVector_t;

__END_DECLS

#endif /* __ELOS_EVENTLOGGING_STORAGEBACKEND_H__ */
