// SPDX-License-Identifier: MIT
#pragma once

typedef enum safuResultE {
    SAFU_RESULT_FAILED = -1,
    SAFU_RESULT_OK = 0,
    SAFU_RESULT_NOT_FOUND = 1,
    SAFU_RESULT_CLOSED = 2,
} safuResultE_t;

// Converts between safuResultE_t and old style int return values (0==OK, -1==FAILURE)
// which is still used by the safuVec* functions as well as a few other places.
// Attention: Information might be lost in the conversion if used improperly
#define RESULT_TO_RETVAL(__value) ((result == SAFU_RESULT_OK) ? 0 : -1)
#define RETVAL_TO_RESULT(__value) ((retval < 0) ? SAFU_RESULT_FAILED : SAFU_RESULT_OK)
