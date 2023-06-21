// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENTQUEUE_TYPES_H
#define ELOS_EVENTQUEUE_TYPES_H

#include "elos/common/types.h"

#define ELOS_EVENTQUEUE_ID_INVALID 0

/*******************************************************************
 * typedef :c:type:`elosId_t` to :c:type:`elosEventQueueId_t`.
 ******************************************************************/
typedef elosId_t elosEventQueueId_t;

/*******************************************************************
 * typedef :c:type:`uint32_t` to :c:type:`elosEventQueueLimitEvents_t`.
 ******************************************************************/
typedef uint32_t elosEventQueueLimitEvents_t;

/**/
typedef struct timespec elosEventQueueLimitTime_t;

#endif /* ELOS_EVENTQUEUE_TYPES_H */
