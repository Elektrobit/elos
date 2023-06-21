// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTBUFFER_EVENTBUFFERINITIALIZE_H__
#define ELOS_EVENTBUFFER_EVENTBUFFERINITIALIZE_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventbuffer/eventbuffer.h"

typedef struct elosUnitTestState {
    elosEventBuffer_t eventBuffer;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventBufferInitializeErrFunc)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventBufferInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventBufferInitializeSuccess)

void elosTestCaseNew(void **state);
void elosTestCaseDelete(void **state);

#endif /* ELOS_EVENTBUFFER_EVENTBUFFERINITIALIZE_H__ */
