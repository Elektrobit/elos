// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H
#define ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "eventprocessor/eventfilternodevector.h"

typedef struct elosTestState {
    elosEventFilterNodeVector_t efev;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorInitializeSuccess)

#endif /* ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H */
