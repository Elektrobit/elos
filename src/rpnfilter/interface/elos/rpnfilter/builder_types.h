// SPDX-License-Identifier: MIT
#pragma once

#include <safu/vector_types.h>

#include "elos/rpnfilter/rpnfilter_types.h"
#include "elos/rpnfilter/step_types.h"

#define ELOS_RPNFILTER_CREATE_STACK_DEFAULT_SIZE 8
#define ELOS_RPNFILTER_CREATE_TOKEN_DEFAULT_SIZE 8
#define ELOS_RPNFILTER_CREATE_STEP_DEFAULT_SIZE  8

#define RPNFILTER_MATCH_SEPARATOR(__var)  ((__var) == ' ')
#define RPNFILTER_MATCH_TERMINATOR(__var) (((__var) == 0) || RPNFILTER_MATCH_SEPARATOR(__var))

// Filter token based data types
typedef enum elosRpnFilterBuilderTokenTypeE {
    RPNFILTER_TOKEN_UNDEFINED = 0,  // Can be anything from a command to a value (e.g. "add", "lt", "42")
    RPNFILTER_TOKEN_LOCATION,       // Anything that starts with a "." (e.g. .1, .event.machineId)
    RPNFILTER_TOKEN_STRING,         // Any sort of string (e.g. "hello world")
    RPNFILTER_TOKEN_REGEX,          // Any sort of regular expression (e.g. "192\.168\.1\.[0-9]{1,3}")
    RPNFILTER_TOKEN_NUMBER,         // Any sort of number (e.g. 2, 10, 42)
    RPNFILTER_TOKEN_COMMAND,        // Any sort of command (e.g. EQ, AND, STRCMP)
    RPNFILTER_TOKEN_SEPARATOR,      // Any sort of separator
    RPNFILTER_TOKEN_TYPE_COUNT,     // Number of token types, useful for range checks
} elosRpnFilterBuilderTokenTypeE_t;

typedef int elosRpnFilterBuilderTokenType_t;

typedef struct elosRpnFilterBuilderTokenEntry {
    elosRpnFilterBuilderTokenType_t type;
    size_t length;
    const char *string;
} elosRpnFilterBuilderTokenEntry_t;

typedef struct elosRpnFilterBuilderToken {
    size_t count;
    elosRpnFilterBuilderTokenEntry_t token[];
} elosRpnFilterBuilderToken_t;

typedef struct elosRpnFilterBuilderStepEntry {
    elosRpnFilterStepType_t type;
    elosRpnFilterBuilderTokenEntry_t const *token;
} elosRpnFilterBuilderStepEntry_t;

typedef enum elosRpnFilterBuilderStatusE {
    RPNFILTER_CREATE_UNDEFINED = 0,
    RPNFILTER_CREATE_INITIALIZED,
    RPNFILTER_CREATE_TOKENIZED,
    RPNFILTER_CREATE_COMPILED,
} elosRpnFilterBuilderStatusE_t;

typedef struct elosRpnFilterBuilder {
    elosRpnFilterBuilderStatusE_t status;
    elosRpnFilterParam_t param;
    safuVec_t token;  // elosRpnFilterBuilderTokenEntry_t
    safuVec_t step;   // elosRpnFilterBuilderStepEntry_t
    safuVec_t stack;  // elosRpnFilterStackEntry_t
} elosRpnFilterBuilder_t;

typedef elosRpnFilterResultE_t (*elosRpnFilterBuilderTokenizeParseFunc_t)(size_t *idx, elosRpnFilterBuilder_t *data);
typedef elosRpnFilterResultE_t (*elosRpnFilterBuilderCompileFunc_t)(elosRpnFilterBuilderTokenEntry_t const *token,
                                                                    elosRpnFilterBuilder_t *data);