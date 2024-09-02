// SPDX-License-Identifier: MIT
#include "SqlBackend.h"

#include <safu/common.h>
#include <safu/log.h>
#include <stdio.h>
#include <stdlib.h>

#include "SqlQueries.h"

#define BACKEND_NAME "SQLite"

static int _checkDatabase(sqlite3 *db) {
    int result = SQLITE_OK;
    char *errMessage = NULL;

    result = sqlite3_exec(db, ELOS_SQL_CREATE_TABLE_EVENTS, NULL, NULL, &errMessage);
    if (result != SQLITE_OK) {
        safuLogErrF("Failed to create 'events' table: %s", errMessage);
        sqlite3_free(errMessage);
    }

    return result;
}

safuResultE_t elosSqlBackendStart(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosSqlBackend_t *sqlBackend = backend->backendData;
    char *sqlConnectionString = (char *)sqlBackend->connectionString;

    safuLogDebugF("start backend %s", backend->name);

    int ret = sqlite3_open(sqlConnectionString, &sqlBackend->db);
    if (ret != SQLITE_OK) {
        safuLogErrF("Failed to open database %s: %i", sqlConnectionString, ret);
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        if (_checkDatabase(sqlBackend->db) != SQLITE_OK) {
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = sqlite3_prepare_v2(sqlBackend->db, ELOS_SQL_INSERT_EVENT, -1, &sqlBackend->insertEvent, NULL);
        if (result != SQLITE_OK) {
            safuLogErrF("Failed to prepare 'insert event' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

safuResultE_t elosSqlBackendPersist(elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    int sqlResult = SQLITE_OK;
    elosSqlBackend_t *sqlBackend = backend->backendData;

    safuLogDebugF("store event: %i, %lu, %i, %lu, %lu, '%s'", event->messageCode, event->classification,
                  event->severity, event->date.tv_sec, event->date.tv_nsec, event->payload);

    sqlResult = sqlite3_bind_int(sqlBackend->insertEvent, 1, event->messageCode);
    if (sqlResult != SQLITE_OK) {
        safuLogErrF("Failed to bind value for 'messageCode' statement: %s", sqlite3_errmsg(sqlBackend->db));
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_int(sqlBackend->insertEvent, 2, event->classification);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'classification' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_int(sqlBackend->insertEvent, 3, event->severity);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'severity' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_int(sqlBackend->insertEvent, 4, event->date.tv_sec);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'date_s' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_int(sqlBackend->insertEvent, 5, event->date.tv_nsec);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'date_ns' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_text(sqlBackend->insertEvent, 6, event->payload, -1, NULL);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'payload' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_int(sqlBackend->insertEvent, 7, event->source.pid);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'pid' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_text(sqlBackend->insertEvent, 8, event->source.appName, -1, NULL);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'appName' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_bind_text(sqlBackend->insertEvent, 9, event->source.fileName, -1, NULL);
        if (sqlResult != SQLITE_OK) {
            safuLogErrF("Failed to bind value for 'fileName' statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        sqlResult = sqlite3_step(sqlBackend->insertEvent);
        if (sqlResult != SQLITE_DONE) {
            safuLogErrF("Failed to execute insert statement: %s", sqlite3_errmsg(sqlBackend->db));
            result = SAFU_RESULT_FAILED;
        }
    }

    sqlResult = sqlite3_reset(sqlBackend->insertEvent);
    if (sqlResult != SQLITE_OK) {
        safuLogErrF("Failed to reset insert statement: %s", sqlite3_errmsg(sqlBackend->db));
        result = SAFU_RESULT_FAILED;
    }

    if (result != SAFU_RESULT_OK) {
        safuLogErr("Failed to persist event");
    }

    return result;
}

safuResultE_t elosSqlBackendFindEvents(elosStorageBackend_t *backend, elosRpnFilter_t *filter,
                                       UNUSED struct timespec const *newest, UNUSED struct timespec const *oldest,
                                       safuVec_t *events) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (backend == NULL || filter == NULL || events == NULL) {
        safuLogErr("Called elosSqlBackendFindEvents with NULL-parameter");
        result = SAFU_RESULT_FAILED;
    } else {
        safuLogErrF("not implemented for '%s'-backend", backend->name);
    }

    return result;
}

safuResultE_t elosSqlBackendShutdown(elosStorageBackend_t *backend) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (backend != NULL) {
        safuLogDebugF("shutdown backend %s", backend->name);

        elosSqlBackend_t *sqlBackend = backend->backendData;
        if (sqlBackend != NULL) {
            sqlite3_finalize(sqlBackend->insertEvent);
            sqlite3_close_v2(sqlBackend->db);
            free(backend->backendData);
        }
        free(backend);
        result = SAFU_RESULT_OK;
    }

    return result;
}

safuResultE_t elosSqlBackendNew(elosStorageBackend_t **backend) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosStorageBackend_t *newBackend = NULL;

    newBackend = safuAllocMem(NULL, sizeof(elosStorageBackend_t));
    if (newBackend == NULL) {
        safuLogErr("safuAllocMem failed");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        memset(newBackend, 0, sizeof(elosStorageBackend_t));
        newBackend->backendData = safuAllocMem(NULL, sizeof(elosSqlBackend_t));
        if (newBackend->backendData == NULL) {
            safuLogCrit("safuAllocMem failed");
            free(newBackend);
            result = SAFU_RESULT_FAILED;
        } else {
            memset(newBackend->backendData, 0, sizeof(elosSqlBackend_t));
            newBackend->name = BACKEND_NAME;
            newBackend->start = elosSqlBackendStart;
            newBackend->persist = elosSqlBackendPersist;
            newBackend->findEvent = elosSqlBackendFindEvents;
            newBackend->shutdown = elosSqlBackendShutdown;
            *backend = newBackend;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
