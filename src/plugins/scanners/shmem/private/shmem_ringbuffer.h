// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/scanner_legacy/scanner.h"
#include "shmem.h"

#define SCANNER_SHMEM_IDX_INVALID UINT16_MAX

safuResultE_t elosScannerRingBufferInitialize(elosScannerLegacySession_t *session);
safuResultE_t elosScannerRingBufferPublish(elosScannerLegacySession_t *session);
