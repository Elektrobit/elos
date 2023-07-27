// SPDX-License-Identifier: MIT

#include <log4c/category.h>
#include <log4c/init.h>
#include <log4c/priority.h>
#include <stdio.h>

#include "elos/libelos/libelos.h"
#include "log4c.h"

typedef enum elosLogContextCodeE {
    ELOS_LOG_CONTEXT_LEVEL_0 = 0,
    ELOS_LOG_CONTEXT_LEVEL_1,
    ELOS_LOG_CONTEXT_LEVEL_2,
} elosLogContextCodeE_t;

extern int elosInitElosLog4cLib(void);

static inline int elosLogInit() {
    return log4c_init();
}

static inline int elosLogClose() {
    return log4c_fini();
}

static inline void elosLogMessage(char *catName, int debugLevel, char *logMessage) {
    log4c_category_log(log4c_category_get(catName), debugLevel, "%s", logMessage);
}

static inline void elosLogSimpleError(elosLogContextCodeE_t logctx, elosEventMessageCodeE_t messageCode,
                                      char *message) {
    if (1100 < messageCode && messageCode < 1110) {
        if (logctx == 0) {
            elosLogMessage("log4c.elos.demo.log4cdemo", LOG4C_PRIORITY_DEBUG, message);
        } else if (logctx == 1) {
            elosLogMessage("log4c.elos.demo.log4cdemo", LOG4C_PRIORITY_ERROR, message);
        } else {
            elosLogMessage("log4c.elos.demo.log4cdemo", LOG4C_PRIORITY_CRIT, message);
        }
    }
}

int main() {
    int ret = 0;

    elosInitElosLog4cLib();

    if (elosLogInit()) {
        printf("log4c_init failed");
        ret = 1;
    } else {
        elosLogSimpleError(ELOS_LOG_CONTEXT_LEVEL_0, ELOS_MSG_CODE_DEBUG_LOG, "elos log message");
        elosLogSimpleError(ELOS_LOG_CONTEXT_LEVEL_1, ELOS_MSG_CODE_DEBUG_LOG, "elos log message");
        elosLogSimpleError(ELOS_LOG_CONTEXT_LEVEL_2, ELOS_MSG_CODE_DEBUG_LOG, "elos log message");

        if (elosLogClose()) {
            printf("log4c fini failed");
        }
    }
    return ret;
}
