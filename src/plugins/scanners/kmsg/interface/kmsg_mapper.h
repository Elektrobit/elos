// SPDX-License-Identifier: MIT
#ifndef ELOS_KMSG_MAPPER_H
#define ELOS_KMSG_MAPPER_H

#include <elos/event/event_types.h>
#include <safu/result.h>
#include <samconf/samconf.h>

#define SCANNER_KMSG_FILE_CREATED 1

typedef struct elosKmsgMapper {
    struct timespec bootTime;
} elosKmsgMapper_t;

struct elosKmsgContext {
    uint32_t kmsgEventId;
    char *kmsgFile;
    int kmsgPollFd;
    char *kmsgStateFile;
    char *kmsgStateBuffer;
    int cmdPollFd;
    bool running;
    int flags;
    elosKmsgMapper_t mapper;
    struct elosPublisher *publisher;
};

safuResultE_t elosKmsgMapperInit(elosKmsgMapper_t *mapper);
safuResultE_t elosKmsgMapperDoMapping(const elosKmsgMapper_t *mapper, elosEvent_t *event, char *kmsgLine);

#endif /* ELOS_KMSG_MAPPER_H */
