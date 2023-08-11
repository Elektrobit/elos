// SPDX-License-Identifier: MIT

#include <safu/log.h>

#include "clientmanager_private.h"
#include "elos/clientmanager/clientmanager.h"

int elosClientManagerStart(elosClientManager_t *ctx) {
    int retval = -1;

    ctx->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    retval = pthread_create(&(ctx->listenThread), 0, elosClientManagerThreadListen, ctx);
    if (retval != 0) {
        safuLogErrErrno("pthread_create failed");
    } else {
        safuLogDebug("listen thread active");
    }

    return retval;
}
