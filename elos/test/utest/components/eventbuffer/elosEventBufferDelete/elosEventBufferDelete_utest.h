// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTBUFFER_EVENTBUFFERDELETE_H__
#define ELOS_EVENTBUFFER_EVENTBUFFERDELETE_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/eventbuffer/eventbuffer.h"

typedef struct elosUnitTestState {
    elosEventBuffer_t *eventBuffer;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventBufferDeleteErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosEventBufferDeleteSuccess)

void elosTestCaseNew(void **state);
void elosTestCaseDelete(void **state);

#endif /* ELOS_EVENTBUFFER_EVENTBUFFERDELETE_H__ */
