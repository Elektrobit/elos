// SPDX-License-Identifier: MIT

#ifndef ELOS_EVENT_SOURCE_NEW_UTEST_H
#define ELOS_EVENT_SOURCE_NEW_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/event/event_source.h"
#include "safu/common.h"

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSourceNewExterrCalloc)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventSourceNewSuccess)

typedef struct elosTestState {
    elosEventSource_t *source;
} elosTestState_t;

#endif /* ELOS_EVENT_SOURCE_NEW_UTEST_H */
