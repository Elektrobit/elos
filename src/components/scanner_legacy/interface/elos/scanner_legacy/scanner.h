// SPDX-License-Identifier: MIT
#ifndef ELOS_SCANNER_H
#define ELOS_SCANNER_H

#include <safu/common.h>
#include <samconf/samconf.h>
#include <stdint.h>

#include "elos/event/event.h"
#include "elos/eventbuffer/types.h"

#define UNUSED            __attribute__((unused))
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

typedef enum elosScannerLegacyResultE {
    SCANNER_ERROR = -1,
    SCANNER_OK = 0,
} elosScannerLegacyResultE_t;

typedef struct elosScannerLegacyParam {
    samconfConfig_t *config;
} elosScannerLegacyParam_t;

typedef struct elosScannerLegacyCallbackData {
    elosEventBuffer_t *eventBuffer;
    void *logAggregator;
} elosScannerLegacyCallbackData_t;

__BEGIN_DECLS

typedef struct elosScannerLegacyCallback {
    safuResultE_t (*eventPublish)(elosScannerLegacyCallbackData_t *data, const elosEvent_t *event);
    safuResultE_t (*eventLog)(elosScannerLegacyCallbackData_t *data, const elosEvent_t *event);
    elosScannerLegacyCallbackData_t scannerCallbackData;
} elosScannerLegacyCallback_t;

typedef struct elosScannerLegacySession {
    const char *name;
    void *context;
    elosEventBuffer_t eventBuffer;
    elosScannerLegacyCallback_t callback;
    const samconfConfig_t *config;
    uint32_t dispatcherSourceid;
    uint32_t running;
} elosScannerLegacySession_t;

elosScannerLegacyResultE_t elosScannerInitialize(elosScannerLegacySession_t *session,
                                                 const elosScannerLegacyParam_t *param);
elosScannerLegacyResultE_t elosScannerRun(elosScannerLegacySession_t *session);
elosScannerLegacyResultE_t elosScannerStop(elosScannerLegacySession_t *session);
elosScannerLegacyResultE_t elosScannerFree(elosScannerLegacySession_t *session);

__END_DECLS

#endif
