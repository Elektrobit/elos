// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "elos/scanner/scanner.h"
#include "shmem.h"

#define SCANNER_SHMEM_IDX_INVALID UINT16_MAX

safuResultE_t elosScannerRingBufferInitialize(elosScannerSession_t *session);
safuResultE_t elosScannerRingBufferPublish(elosScannerSession_t *session);
