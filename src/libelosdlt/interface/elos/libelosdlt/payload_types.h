// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>
#include <stdio.h>

#define MESSAGE_ID_LENGTH 4

__BEGIN_DECLS

/*******************************************************************
 * a dlt payload struct
 *
 * Members:
 *     messageId: unique id to parse dlt message
 *     data: body of the dlt messages
 *     size: size of payload data
 ******************************************************************/
typedef struct elosDltPayload {
    uint32_t messageId;
    unsigned char *data;
    ssize_t size;
} elosDltPayload_t;

__END_DECLS
