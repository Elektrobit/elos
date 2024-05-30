// SPDX-License-Identifier: MIT
#ifndef ELOS_SCANNERMANAGER_H_
#define ELOS_SCANNERMANAGER_H_

#include <samconf/samconf.h>
#include <stdint.h>

#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventlogging/LogAggregator.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/scanner_legacy/scanner.h"
#include "safu/log.h"

#define SCANNER_ENTRIES_INIT 8

typedef enum elosScannerStateLegacyE {
    SCANNER_INVALID = 0,
    SCANNER_PARTIALLY_INITIALIZED,
    SCANNER_INITIALIZED,
    SCANNER_RUNNING,
    SCANNER_STOPPED,
    SCANNER_ERROR_FATAL,
    SCANNER_ERROR_FATAL_DLOPEN,
    SCANNER_ERROR_FATAL_DLSYM,
} elosScannerStateLegacyE_t;

typedef enum elosScannerManagerLegacyErrorCodeE {
    NO_ERROR = 0,
    NO_FATAL_ERRORS = -1,
    ERROR_ALLOCATION = -2,
    ERROR_VEC_MANIPULATION = -3,
    ERROR_SCANNER_PATH_INVALID = -4,
    ERROR_SCANNER_PATH_EMPTY = -5,
    ERROR_ZERO_POINTER = -6,
    ERROR_FUNCTION_CALL = -7,
} elosScannerManagerLegacyErrorCodeE_t;

typedef struct elosScannerManagerLegacyContext {
    safuVec_t scannerEntry;
    samconfConfig_t *config;
    elosLogAggregator_t *logAggregator;
    elosEventDispatcher_t *eventDispatcher;
} elosScannerManagerLegacyContext_t;

typedef struct elosScannerManagerLegacyParam {
    samconfConfig_t *config;
    elosLogAggregator_t *logAggregator;
    elosEventDispatcher_t *eventDispatcher;
} elosScannerManagerLegacyParam_t;

__BEGIN_DECLS

typedef struct elosScannerEntryLegacy {
    void *dlHandle;
    char *file;
    pthread_t thread;
    elosScannerStateLegacyE_t state;
    elosScannerLegacySession_t session;
    elosScannerLegacyResultE_t (*funcInitialize)(elosScannerLegacySession_t *session,
                                                 const elosScannerLegacyParam_t *param);
    elosScannerLegacyResultE_t (*funcRun)(elosScannerLegacySession_t *session);
    elosScannerLegacyResultE_t (*funcStop)(elosScannerLegacySession_t *session);
    elosScannerLegacyResultE_t (*funcFree)(elosScannerLegacySession_t *session);
} elosScannerEntryLegacy_t;

int elosScannerManagerLegacyStart(elosScannerManagerLegacyContext_t *context,
                                  elosScannerManagerLegacyParam_t const *param);
int elosScannerManagerLegacyStop(elosScannerManagerLegacyContext_t *context);

__END_DECLS

#endif /* ELOS_SCANNERMANAGER_H_ */
