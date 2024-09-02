// SPDX-License-Identifier: MIT
#include "NoSqlBackend.h"

#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <safu/common.h>
#include <safu/log.h>

#include "elos/event/event.h"

#define BACKEND_NAME "NOSQL"

safuResultE_t elosNoSqlBackendStart(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosNoSqlBackend_t *noSqlBackend = backend->backendData;

    safuLogDebugF("start backend %s", backend->name);

    mongoc_init();

    noSqlBackend->client = mongoc_client_new(noSqlBackend->connectionString);
    if (noSqlBackend->client == NULL) {
        safuLogErrF("Failed to connect to : %s", noSqlBackend->connectionString);
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        noSqlBackend->collection = mongoc_client_get_collection(noSqlBackend->client, "elosd", "events");
        if (noSqlBackend->collection == NULL) {
            safuLogErrF("Failed to get database : %s", "elsod");
            result = SAFU_RESULT_FAILED;
        }
    }

    safuLogDebugF("%s connected", backend->name);
    return result;
}

safuResultE_t elosNoSqlBackendPersist(elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosNoSqlBackend_t *noSqlBackend = backend->backendData;
    bson_error_t error = {0};
    bson_oid_t objectId = {0};
    bson_t *bEvent = NULL;

    safuLogDebugF("store event: %i, %lu, %i, %lu, %lu, '%s'", event->messageCode, event->classification,
                  event->severity, event->date.tv_sec, event->date.tv_nsec, event->payload);

    bEvent = bson_new();
    if (bEvent == NULL) {
        safuLogErr("Failed to create new bson object");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        bson_oid_init(&objectId, NULL);
        BSON_APPEND_OID(bEvent, "_id", &objectId);
        BSON_APPEND_INT32(bEvent, "messageCode", event->messageCode);
        BSON_APPEND_INT64(bEvent, "classification", event->classification);
        BSON_APPEND_INT32(bEvent, "severity", event->severity);
        BSON_APPEND_INT64(bEvent, "date_s", event->date.tv_sec);
        BSON_APPEND_INT64(bEvent, "date_ns", event->date.tv_nsec);
        if (event->payload != NULL) {
            BSON_APPEND_UTF8(bEvent, "payload", event->payload);
        }

        BSON_APPEND_INT64(bEvent, "pid", event->source.pid);
        if (event->source.appName != NULL) {
            BSON_APPEND_UTF8(bEvent, "appName", event->source.appName);
        }
        if (event->source.fileName != NULL) {
            BSON_APPEND_UTF8(bEvent, "fileName", event->source.fileName);
        }

        bool success = mongoc_collection_insert_one(noSqlBackend->collection, bEvent, NULL, NULL, &error);
        if (!success) {
            result = SAFU_RESULT_FAILED;
            safuLogErrF("Failed to persist event: %s", error.message);
        }
    }

    bson_destroy(bEvent);
    return result;
}

static safuResultE_t _convertBsonEventToEvent(const bson_t *bEvent, elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    bson_iter_t iter = {0};

    if (bson_iter_init(&iter, bEvent)) {
        while (bson_iter_next(&iter)) {
            if (BSON_ITER_IS_KEY(&iter, "messageCode")) event->messageCode = bson_iter_as_int64(&iter);
            if (BSON_ITER_IS_KEY(&iter, "classification")) event->classification = bson_iter_as_int64(&iter);
            if (BSON_ITER_IS_KEY(&iter, "severity")) event->severity = bson_iter_as_int64(&iter);
            if (BSON_ITER_IS_KEY(&iter, "date_s")) event->date.tv_sec = bson_iter_as_int64(&iter);
            if (BSON_ITER_IS_KEY(&iter, "date_ns")) event->date.tv_nsec = bson_iter_as_int64(&iter);
            if (BSON_ITER_IS_KEY(&iter, "payload")) event->payload = bson_iter_dup_utf8(&iter, NULL);
            if (BSON_ITER_IS_KEY(&iter, "pid")) event->source.pid = bson_iter_as_int64(&iter);
            if (BSON_ITER_IS_KEY(&iter, "appName")) event->source.appName = bson_iter_dup_utf8(&iter, NULL);
            if (BSON_ITER_IS_KEY(&iter, "fileName")) event->source.fileName = bson_iter_dup_utf8(&iter, NULL);
        }
    }

    return result;
}

static safuResultE_t elosNoSqlBackendFilterEvent(elosRpnFilter_t *filter, safuVec_t *events, const bson_t *bEvent) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosRpnFilterResultE_t filterResult;

    elosEvent_t *event = NULL;
    result = elosEventNew(&event);
    if (result != SAFU_RESULT_OK || event == NULL) {
        safuLogErr("allocate event failed failed");
    } else {
        safuLogDebug("Event created");

        _convertBsonEventToEvent(bEvent, event);

        filterResult = elosEventFilterExecute(filter, NULL, event);
        if (filterResult == RPNFILTER_RESULT_MATCH) {
            safuVecPush(events, event);
            free(event);
            safuLogDebug("Append event: Match");
            result = SAFU_RESULT_OK;
        } else if (filterResult == RPNFILTER_RESULT_NO_MATCH) {
            elosEventDelete(event);
            safuLogDebug("Free Event: No Match");
            result = SAFU_RESULT_OK;
        } else {
            safuLogErr("elosEventFilterExecute failed");
            elosEventDelete(event);
            safuLogDebug("Free Event: Error");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosNoSqlBackendFindEvents(elosStorageBackend_t *backend, elosRpnFilter_t *filter,
                                         struct timespec const *newest, struct timespec const *oldest,
                                         safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosNoSqlBackend_t *noSqlBackend = NULL;
    bson_t *selectAll = NULL;

    if (backend == NULL || filter == NULL || events == NULL) {
        safuLogErr("Called elosNoSqlBackendFindEvents with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK && backend->backendData != NULL) {
        noSqlBackend = backend->backendData;
    } else {
        safuLogErr("Called elosNoSqlBackendFindEvents with uninitialized backend data");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        selectAll = bson_new();
        if (selectAll == NULL) {
            safuLogErr("bson_new failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(noSqlBackend->collection, selectAll, NULL, NULL);
        const bson_t *bEvent = NULL;
        while (mongoc_cursor_next(cursor, &bEvent)) {
            result = elosNoSqlBackendFilterEvent(filter, events, bEvent);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosNoSqlBackendFilterEvent failed");
            }
        }
        mongoc_cursor_destroy(cursor);
    }

    bson_destroy(selectAll);
    return result;
}

safuResultE_t elosNoSqlBackendShutdown(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend != NULL) {
        safuLogDebugF("shutdown backend %s", backend->name);

        elosNoSqlBackend_t *noSqlBackend = backend->backendData;
        if (noSqlBackend != NULL) {
            mongoc_collection_destroy(noSqlBackend->collection);
            mongoc_client_destroy(noSqlBackend->client);
            mongoc_cleanup();
            free(backend->backendData);
        }
        free(backend);
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosNoSqlBackendNew(elosStorageBackend_t **backend) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosStorageBackend_t *newBackend = NULL;

    newBackend = safuAllocMem(NULL, sizeof(elosStorageBackend_t));
    if (newBackend == NULL) {
        safuLogErr("safuAllocMem failed");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        memset(newBackend, 0, sizeof(elosStorageBackend_t));
        newBackend->backendData = safuAllocMem(NULL, sizeof(elosNoSqlBackend_t));
        if (newBackend->backendData == NULL) {
            safuLogCrit("safuAllocMem failed");
            free(newBackend);
            result = SAFU_RESULT_FAILED;
        } else {
            memset(newBackend->backendData, 0, sizeof(elosNoSqlBackend_t));
            newBackend->name = BACKEND_NAME;
            newBackend->start = elosNoSqlBackendStart;
            newBackend->persist = elosNoSqlBackendPersist;
            newBackend->findEvent = elosNoSqlBackendFindEvents;
            newBackend->shutdown = elosNoSqlBackendShutdown;
            *backend = newBackend;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
