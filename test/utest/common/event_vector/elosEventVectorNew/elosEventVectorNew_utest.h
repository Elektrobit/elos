// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_VECTOR_NEW_UTEST_H
#define ELOS_EVENT_VECTOR_NEW_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/event/event_vector.h"

typedef struct {
    elosEventVector_t *eventVector;
    uint32_t numElements;
} elosUteststateT_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorNewErrEventVector)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorNewErrElements)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorExtErrAllocMem)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorNewSuccess)

#endif /* ELOS_EVENT_VECTOR_NEW_UTEST_H */
