// SPDX-License-Identifier: MIT
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum elosRpnFilterResultE {
    RPNFILTER_RESULT_ERROR = -1,
    RPNFILTER_RESULT_OK = 0,
    RPNFILTER_RESULT_MATCH,
    RPNFILTER_RESULT_NO_MATCH,
    RPNFILTER_RESULT_NOT_FOUND,
    // Specific error codes
    RPNFILTER_RESULT_ERROR_NUMOP_TYPE,
    RPNFILTER_RESULT_ERROR_NUMCMP_TYPE,
} elosRpnFilterResultE_t;

typedef enum elosRpnFilterValueTypeE {
    RPNFILTER_VALUE_UNDEFINED = 0,
    RPNFILTER_VALUE_BINARY,
    RPNFILTER_VALUE_BOOL,
    RPNFILTER_VALUE_I32,
    RPNFILTER_VALUE_U32,
    RPNFILTER_VALUE_I64,
    RPNFILTER_VALUE_U64,
    RPNFILTER_VALUE_STRING,
    RPNFILTER_VALUE_REGEX,
    RPNFILTER_VALUE_TYPE_COUNT  // Useful for range checks
} elosRpnFilterValueTypeE_t;

// We need to define a separate type from the enum here as the RpnFilter
// is intended to be extended by other components (like EventFilter).
// These have their own value types in addition to elosRpnFilterValueTypeE_t,
// as such we define this value as a standard integer to avoid warnings.
typedef int elosRpnFilterValueType_t;

typedef struct elosRpnFilterParam {
    const char *filterString;
} elosRpnFilterParam_t;

typedef union elosRpnFilterStackDataU {
    bool b;
    int e;
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    char *str;
    void *ptr;
} elosRpnFilterStackDataU_t;

typedef struct elosRpnFilterStackEntry {
    elosRpnFilterValueType_t type;
    size_t bytes;
    elosRpnFilterStackDataU_t data;
} elosRpnFilterStackEntry_t;

// These macros will be replaced with something nicer in the future

// Creates a stack with __num_entries entries, the stack will be known under the var-name given to __stack
#define ELOS_FILTERSTACK_NEW(__stack, __num_entries)                                                                  \
    uint8_t __stack##Buffer[sizeof(elosRpnFilterStack_t) + (sizeof(elosRpnFilterStackEntry_t) * (__num_entries))] = { \
        0};                                                                                                           \
    elosRpnFilterStack_t *(__stack) = (elosRpnFilterStack_t *)__stack##Buffer;                                        \
    (__stack)->count = (__num_entries);

// Set the __idx'th entry's contents of elosRpnFilterStack "__stack" to __type, __bytes, __data ...
// ... when __data is a pointer
#define ELOS_FILTERSTACK_SET(__stack, __idx, __type, __bytes, __data) \
    (__stack)->entry[__idx].type = (__type);                          \
    (__stack)->entry[__idx].bytes = (__bytes);                        \
    (__stack)->entry[__idx].data.ptr = (__data);

// ... when __data is a bool
#define ELOS_FILTERSTACK_SET_BOOL(__stack, __idx, __data) \
    (__stack)->entry[__idx].type = RPNFILTER_VALUE_BOOL;  \
    (__stack)->entry[__idx].bytes = sizeof(bool);         \
    (__stack)->entry[__idx].data.b = (__data);

// ... when __data is a signed 32 bit int
#define ELOS_FILTERSTACK_SET_I32(__stack, __idx, __data) \
    (__stack)->entry[__idx].type = RPNFILTER_VALUE_I32;  \
    (__stack)->entry[__idx].bytes = sizeof(int32_t);     \
    (__stack)->entry[__idx].data.i32 = (__data);

// ... when __data is a unsigned 32 bit int
#define ELOS_FILTERSTACK_SET_U32(__stack, __idx, __data) \
    (__stack)->entry[__idx].type = RPNFILTER_VALUE_U32;  \
    (__stack)->entry[__idx].bytes = sizeof(uint32_t);    \
    (__stack)->entry[__idx].data.u32 = (__data);

typedef struct elosFilterStack {
    size_t count;
    elosRpnFilterStackEntry_t entry[];
} elosRpnFilterStack_t;

// Filter based data types
typedef enum elosRpnFilterStateE { RPNFILTER_UNDEFINED = 0, RPNFILTER_FINALIZED = 1 } elosRpnFilterStateE_t;

struct elosRpnFilterStep;
typedef struct elosRpnFilterStep elosRpnFilterStep_t;

#define ELOS_RPNFILTER_INIT \
    { 0 }

// Note: elosRpnFilterStep_t is defined in rpnfilter/builder.h
typedef struct elosRpnFilter {
    elosRpnFilterStateE_t state;
    size_t memorySize;
    void *memory;
    elosRpnFilterStep_t *steps;
    elosRpnFilterStack_t *values;
} elosRpnFilter_t;
