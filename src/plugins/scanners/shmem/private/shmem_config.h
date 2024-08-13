// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/types.h>
#include <safu/result.h>

#include "shmem.h"

#ifndef SCANNER_SHMEM_TIMEOUT_SEC
#define SCANNER_SHMEM_TIMEOUT_SEC 0
#endif

#ifndef SCANNER_SHMEM_TIMEOUT_NSEC
#define SCANNER_SHMEM_TIMEOUT_NSEC (100 * 1000 * 1000)
#endif

safuResultE_t elosScannerConfigLoad(elosPlugin_t *plugin);
