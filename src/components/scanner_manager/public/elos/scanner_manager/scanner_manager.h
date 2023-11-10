// SPDX-License-Identifier: MIT
#ifndef ELOS_SCANNERMANAGER_H_
#define ELOS_SCANNERMANAGER_H_

#include <samconf/samconf.h>
#include <stdint.h>

#include "elos/connectionmanager/connectionmanager.h"
#include "elos/eventlogging/LogAggregator.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/scanner/scanner.h"
#include "safu/log.h"

#define SCANNER_ENTRIES_INIT 8

typedef enum elosScannerStateE {
    SCANNER_INVALID = 0,
    SCANNER_PARTIALLY_INITIALIZED,
    SCANNER_INITIALIZED,
    SCANNER_RUNNING,
    SCANNER_STOPPED,
    SCANNER_ERROR_FATAL,
    SCANNER_ERROR_FATAL_DLOPEN,
    SCANNER_ERROR_FATAL_DLSYM,
} elosScannerStateE_t;

typedef enum elosScannerManagerErrorCodeE {
    NO_ERROR = 0,
    NO_FATAL_ERRORS = -1,
    ERROR_ALLOCATION = -2,
    ERROR_VEC_MANIPULATION = -3,
    ERROR_SCANNER_PATH_INVALID = -4,
    ERROR_SCANNER_PATH_EMPTY = -5,
    ERROR_ZERO_POINTER = -6,
    ERROR_FUNCTION_CALL = -7,
} elosScannerManagerErrorCodeE_t;

typedef struct elosScannerEntry {
    void *dlHandle;
    char *file;
    pthread_t thread;
    elosScannerStateE_t state;
    elosScannerSession_t session;
    elosScannerResultE_t (*funcInitialize)(elosScannerSession_t *session, const elosScannerParam_t *param);
    elosScannerResultE_t (*funcRun)(elosScannerSession_t *session);
    elosScannerResultE_t (*funcStop)(elosScannerSession_t *session);
    elosScannerResultE_t (*funcFree)(elosScannerSession_t *session);
} elosScannerEntry_t;

typedef struct elosScannerManagerContext {
    safuVec_t scannerEntry;
    samconfConfig_t *config;
    elosLogAggregator_t *logAggregator;
    elosEventDispatcher_t *eventDispatcher;
} elosScannerManagerContext_t;

typedef struct elosScannerManagerParam {
    samconfConfig_t *config;
    elosLogAggregator_t *logAggregator;
    elosEventDispatcher_t *eventDispatcher;
} elosScannerManagerParam_t;

int elosScannerManagerStart(elosScannerManagerContext_t *context, elosScannerManagerParam_t const *param);
int elosScannerManagerStop(elosScannerManagerContext_t *context);

#endif /* ELOS_SCANNERMANAGER_H_ */
