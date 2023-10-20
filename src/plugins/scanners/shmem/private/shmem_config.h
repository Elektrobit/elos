// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>

#include "shmem.h"

#ifndef SCANNER_SHMEM_NAME
#define SCANNER_SHMEM_NAME "shmem"
#endif

#ifndef SCANNER_SHMEM_CONFIG_PATH
#define SCANNER_SHMEM_CONFIG_PATH ELOS_CONFIG_SCANNER "Shmem"
#endif

#ifndef SCANNER_SHMEM_TIMEOUT_SEC
#define SCANNER_SHMEM_TIMEOUT_SEC 0
#endif

#ifndef SCANNER_SHMEM_TIMEOUT_NSEC
#define SCANNER_SHMEM_TIMEOUT_NSEC (100 * 1000 * 1000)
#endif

safuResultE_t elosScannerConfigLoad(elosScannerContextShmem_t *context);
