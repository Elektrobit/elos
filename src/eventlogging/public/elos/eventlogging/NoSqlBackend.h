// SPDX-License-Identifier: MIT
#ifndef __ELOS_NOSQLBACKEND_H__
#define __ELOS_NOSQLBACKEND_H__

#include "elos/eventfilter/eventfilter.h"
#include "elos/eventlogging/StorageBackend.h"
#include "safu/common.h"
#include "safu/vector.h"

safuResultE_t elosNoSqlBackendNew(elosStorageBackend_t **backend);

#endif /* __ELOS_NOSQLBACKEND_H__ */
