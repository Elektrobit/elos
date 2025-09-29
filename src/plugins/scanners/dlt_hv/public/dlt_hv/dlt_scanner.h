// SPDX-License-Identifier: MIT

#pragma once

#include <elos/libelosplugin/types.h>

#include "dlt_hv/scanner.h"

safuResultE_t elosDltScannerInit(elosPlugin_t *plugin);
safuResultE_t elosDltScannerOpenBuffer(elosDltScanner_t *dlt);
void *elosDltScannerLoop(elosDltScanner_t *dlt);
safuResultE_t elosDltPublisherLoop(elosPlugin_t *plugin);
safuResultE_t elosDltScannerStopLoops(elosDltScanner_t *dlt);
safuResultE_t elosDltScannerFree(elosPlugin_t *plugin);
