// SPDX-License-Identifier: MIT
#ifndef __ELOS_SQLBACKEND_H__
#define __ELOS_SQLBACKEND_H__

#include "elos/eventlogging/StorageBackend.h"
#include "safu/common.h"
#include "safu/vector.h"

safuResultE_t elosSqlBackendNew(elosStorageBackend_t **backend);

#endif /* __ELOS_SQLBACKEND_H__ */
