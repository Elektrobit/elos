// SPDX-License-Identifier: MIT

#pragma once

#include <safu/log.h>
#include <stdlib.h>

#include "elos/libelosplugin/libelosplugin.h"
#include "safu/common.h"

typedef struct elosOomKillerScanner {
    struct elosSubscriber *oomEventSubscriber;
    struct elosPublisher *oomEventPublisher;
    const elosSubscription_t *oomEventSubscription;
} elosOomKillerScanner_t;


safuResultE_t elosOomKillerScannerNew(elosOomKillerScanner_t **oomKillerScanner, elosPlugin_t *plugin);

safuResultE_t elosOomKillerScannerStart(elosOomKillerScanner_t *oomKillerScanner, elosPlugin_t *plugin);

safuResultE_t elosOomKillerScannerStop(elosOomKillerScanner_t *oomKillerScanner, elosPlugin_t *plugin);
