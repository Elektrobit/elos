// SPDX-License-Identifier: MIT
#ifndef ELOS_LOGLINE_MAPPER_H
#define ELOS_LOGLINE_MAPPER_H

#include <samconf/samconf.h>

#include "elos/event/event.h"
#include "safu/common.h"
#include "safu/vector.h"

typedef struct elosLoglineMapper {
    safuVec_t filterList;
} elosLoglineMapper_t;

safuResultE_t elosLoglineMapperInit(elosLoglineMapper_t *mapper, const samconfConfig_t *config);
safuResultE_t elosLoglineMapperDeleteMembers(elosLoglineMapper_t *mapper);
safuResultE_t elosLoglineMapperDoMapping(elosLoglineMapper_t *mapper, elosEvent_t *event, const char *logLine);

#endif
