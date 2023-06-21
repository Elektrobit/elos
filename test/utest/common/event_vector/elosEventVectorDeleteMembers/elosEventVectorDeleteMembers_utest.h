// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_VECTOR_DELETE_MEMBERS_UTEST_H
#define ELOS_EVENT_VECTOR_DELETE_MEMBERS_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/event/event_vector.h"

typedef struct {
    elosEventVector_t eventVector;
} elosUteststateT_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorDeleteMembersErrEventVector)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventVectorDeleteMembersSuccess)

#endif /* ELOS_EVENT_VECTOR_DELETE_MEMBERS_UTEST_H */
