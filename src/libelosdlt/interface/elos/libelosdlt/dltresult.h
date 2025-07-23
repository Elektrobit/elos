// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>

__BEGIN_DECLS

/*******************************************************************
 * The possible results when parsing an dlt message using elos dlt parser
 ******************************************************************/
typedef enum elosDltParseResultE {
    ELOS_DLT_INVALID_DLT_VERSION = -4,
    ELOS_DLT_INVALID_DLT_MSG = -3,
    ELOS_DLT_INCOMPLETE_DLT_HDR = -2,
    ELOS_DLT_INCOMPLETE_DLT_MSG = -1,
    ELOS_DLT_RESULT_OK = 0,
} elosDltParseResultE_t;

__END_DECLS
