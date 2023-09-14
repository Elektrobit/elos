// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H
#define ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>

#include "elos/eventprocessor/eventfilternode.h"

typedef struct elosTestState {
    elosEventFilterNode_t efe;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeInitializeErrDoubleInit)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeInitializeErrMutex)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeInitializeSuccess)

#endif /* ELOS_EVENTFILTERENTRYINITIALIZE_UTEST_H */
