// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTER_STEP_H
#define ELOS_EVENTFILTER_STEP_H

#include <sys/cdefs.h>
#include "elos/rpnfilter/step_types.h"

__BEGIN_DECLS

// Filter step based data types
typedef enum elosEventFilterStepTypeE {
    EVENTFILTER_STEP_LDI_EV = RPNFILTER_STEP_TYPE_COUNT,
    EVENTFILTER_STEP_TYPE_COUNT
} elosEventFilterStepTypeE_t;

__END_DECLS

#endif /* ELOS_EVENTFILTER_STEP_H */
