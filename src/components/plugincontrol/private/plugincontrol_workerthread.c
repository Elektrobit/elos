// SPDX-License-Identifier: MIT

#include <pthread.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "elos/plugincontrol/plugincontrol.h"
#include "elos/plugincontrol/thread.h"
#include "safu/log.h"
#include "safu/mutex.h"

void *elosPluginControlWorkerThread(void *data) {
    elosPluginControl_t *control = (elosPluginControl_t *)data;
    safuResultE_t result = SAFU_RESULT_OK;
    int retVal;

    result = control->pluginConfig->load(&control->context);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("plugin load function call failed");
    }

    retVal = eventfd_write(control->context.sync, 1);
    if (retVal < 0) {
        safuLogErrErrno("pthread_cond_wait failed");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        eventfd_t efdVal = 0;

        retVal = eventfd_read(control->sync, &efdVal);
        if (retVal < 0) {
            safuLogErrErrno("eventfd_read (sync) failed");
            result = SAFU_RESULT_FAILED;
        } else {
            result = control->pluginConfig->start(&control->context);
            if (result != SAFU_RESULT_OK) {
                safuLogErr("plugin load function call failed");
            } else {
                retVal = eventfd_write(control->context.sync, 1);
                if (retVal < 0) {
                    safuLogErrErrno("eventfd_write (worker.sync) failed");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    if (result != SAFU_RESULT_OK) {
        atomic_fetch_or(&control->flags, ELOS_PLUGINCONTROL_FLAG_PLUGINERROR_BIT);
    }

    return NULL;
}
