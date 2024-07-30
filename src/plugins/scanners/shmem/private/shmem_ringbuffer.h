// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/types.h>
#include <safu/result.h>

#include "shmem.h"

#define SCANNER_SHMEM_IDX_INVALID UINT16_MAX

safuResultE_t elosScannerRingBufferInitialize(elosPlugin_t *plugin);
safuResultE_t elosScannerRingBufferPublish(elosPlugin_t *plugin);
