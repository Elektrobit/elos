// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTBUFFER_EVENTBUFFERREAD_H__
#define ELOS_EVENTBUFFER_EVENTBUFFERREAD_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventbuffer/eventbuffer.h"

typedef struct elosUnitTestState {
    elosEventBuffer_t eventBuffer;
    elosEventVector_t eventPtrVector;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventBufferReadErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventBufferReadSuccess)

void elosTestCaseNew(void **state);
void elosTestCaseDelete(void **state);

#endif /* ELOS_EVENTBUFFER_EVENTBUFFERREAD_H__ */
