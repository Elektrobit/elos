// SPDX-License-Identifier: MIT
#ifndef ELOS_SCANNER_H
#define ELOS_SCANNER_H

#include <safu/common.h>
#include <samconf/samconf.h>
#include <stdint.h>

#include "elos/event/event.h"

#define UNUSED            __attribute__((unused))
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

typedef enum elosScannerResultE {
    SCANNER_ERROR = -1,
    SCANNER_OK = 0,
} elosScannerResultE_t;

typedef struct elosScannerParam {
    samconfConfig_t *config;
} elosScannerParam_t;

typedef struct elosScannerCallbackData {
    void *eventProcessor;
    void *logAggregator;
} elosScannerCallbackData_t;

typedef struct elosScannerCallback {
    safuResultE_t (*eventPublish)(elosScannerCallbackData_t *data, const elosEvent_t *event);
    safuResultE_t (*eventLog)(elosScannerCallbackData_t *data, const elosEvent_t *event);
    elosScannerCallbackData_t scannerCallbackData;
} elosScannerCallback_t;

typedef struct elosScannerSession {
    const char *name;
    void *context;
    elosScannerCallback_t callback;
    const samconfConfig_t *config;
    uint32_t dispatcherSourceid;
    uint32_t running;
} elosScannerSession_t;

elosScannerResultE_t elosScannerInitialize(elosScannerSession_t *session, const elosScannerParam_t *param);
elosScannerResultE_t elosScannerRun(elosScannerSession_t *session);
elosScannerResultE_t elosScannerStop(elosScannerSession_t *session);
elosScannerResultE_t elosScannerFree(elosScannerSession_t *session);

#endif
