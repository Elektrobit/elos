// SPDX-License-Identifier: MIT
#ifndef ELOS_KMSG_MAPPER_H
#define ELOS_KMSG_MAPPER_H

#include <samconf/samconf.h>

#include "elos/event/event.h"
#include "elos/event/event_message_codes.h"

//#define ELOS_MSG_CODE_KMSG 1111

typedef struct elosKmsgMapper {
    struct timespec bootTime;
} elosKmsgMapper_t;

safuResultE_t elosKmsgMapperInit(elosKmsgMapper_t *mapper);
safuResultE_t elosKmsgMapperDoMapping(const elosKmsgMapper_t *mapper, elosEvent_t *event, char *kmsgLine);

#endif /* ELOS_KMSG_MAPPER_H */
