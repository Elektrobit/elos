// SPDX-License-Identifier: MIT
#ifndef ELOS_RPNFILTER_STEP_H
#define ELOS_RPNFILTER_STEP_H

#include <stddef.h>

// Filter step based data types
typedef enum elosRpnFilterStepEntryE {
    RPNFILTER_STEP_UNDEFINED = 0,

    RPNFILTER_STEP_EQ,
    RPNFILTER_STEP_NE,
    RPNFILTER_STEP_LE,
    RPNFILTER_STEP_GE,
    RPNFILTER_STEP_LT,
    RPNFILTER_STEP_GT,

    RPNFILTER_STEP_AND,
    RPNFILTER_STEP_OR,
    RPNFILTER_STEP_XOR,
    RPNFILTER_STEP_NOT,
    RPNFILTER_STEP_ADD,
    RPNFILTER_STEP_SUB,
    RPNFILTER_STEP_DIV,
    RPNFILTER_STEP_MUL,

    RPNFILTER_STEP_STRCMP,

    RPNFILTER_STEP_REGEX,

    RPNFILTER_STEP_LDP,
    RPNFILTER_STEP_LDV,

    RPNFILTER_STEP_TYPE_COUNT,  // Number of step types, useful for range checks
} elosRpnFilterStepTypeE_t;

typedef int elosRpnFilterStepType_t;

typedef struct elosRpnFilterStepEntry {
    elosRpnFilterStepType_t type;
} elosRpnFilterStepEntry_t;

struct elosRpnFilterStep {
    size_t count;
    elosRpnFilterStepEntry_t entry[];
};

#endif /* ELOS_RPNFILTER_STEP_H */
