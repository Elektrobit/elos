// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTERNODEVECTORREMOVEBYID_UTEST_H
#define ELOS_EVENTFILTERNODEVECTORREMOVEBYID_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "eventprocessor/eventfilternodevector.h"

typedef struct elosTestState {
    elosEventFilterNodeVector_t efev;
} elosTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorRemoveByIdSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorRemoveByIdVectorNull)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorRemoveByIdInvalidId)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorRemoveByIdFindFailed)
TEST_CASE_FUNC_PROTOTYPES(elosTestEloEventFilterNodeVectorRemoveByIdRemoveFailed)

#endif /* ELOS_EVENTFILTERNODEVECTORREMOVEBYID_UTEST_H */