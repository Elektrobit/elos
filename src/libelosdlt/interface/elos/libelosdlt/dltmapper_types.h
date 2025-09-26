// SPDX-License-Identifier: MIT
#pragma once

#include <safu/types.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

__BEGIN_DECLS

/*******************************************************************
 * An elos dlt mapper struct
 *
 * Members:
 *     fileName : default file name for dlt elos event
 *     appId : default application id for dlt elos event
 *     hardwareid : default hardware id for dlt elos event
 *     pid : default pid for dlt elos event
 *     startTime : timestamp of system was started as Wallclock format since epoch
 ******************************************************************/
typedef struct elosDltMapper {
    char *fileName;
    char *appId;
    char *hardwareid;
    pid_t pid;
    struct timespec startTime;
} elosDltMapper_t;

__END_DECLS
