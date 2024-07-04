// SPDX-License-Identifier: MIT
#ifndef __ELOS_TYPES_H__
#define __ELOS_TYPES_H__

#include <stdint.h>

#define ELOS_ID_INVALID 0

typedef uint32_t elosId_t;

/*
 * Represent a subscription as part of a eventQueue and corresponding event filter.
 *
 * The subscription shall be used as identifier to deal with subscriptions.
 * External APIs shall not rely on the structure and members of the
 * subscription.
 *
 * Member:
 *     - eventQueueId: elos internal queue identifier
 *     - eventFilterNodeIf: elos internal filter identifier
 */
typedef struct elosSubscription {
    elosId_t eventQueueId;
    elosId_t eventFilterNodeId;
} elosSubscription_t;

#endif /* __ELOS_TYPES_H__ */
