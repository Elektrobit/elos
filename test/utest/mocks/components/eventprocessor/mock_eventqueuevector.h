// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTPROCESSOR_EVENTQUEUEVECTOR_MOCK_H
#define ELOS_EVENTPROCESSOR_EVENTQUEUEVECTOR_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_EVENTPROCESSOR_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "eventprocessor/eventqueuevector.h"

MOCK_FUNC_PROTOTYPE(elosEventQueueVectorPushByParam, safuResultE_t, elosEventQueueVector_t *eventQueueVector,
                    elosEventQueueParam_t const *param)

#endif /* ELOS_EVENTPROCESSOR_EVENTQUEUEVECTOR_MOCK_H */
