// SPDX-License-Identifier: MIT
#ifndef ELOS_KMSG_MAPPER_H
#define ELOS_KMSG_MAPPER_H

#include <elos/event/event_types.h>
#include <safu/result.h>
#include <samconf/samconf.h>

typedef struct elosKmsgMapper {
    struct timespec bootTime;
} elosKmsgMapper_t;

safuResultE_t elosKmsgMapperInit(elosKmsgMapper_t *mapper);
safuResultE_t elosKmsgMapperDoMapping(const elosKmsgMapper_t *mapper, elosEvent_t *event, char *kmsgLine);

#endif /* ELOS_KMSG_MAPPER_H */
