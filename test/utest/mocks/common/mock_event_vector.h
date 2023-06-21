// SPDX-License-Identifier: MIT

#ifndef __MOCK_EVENT_VECTOR_H__
#define __MOCK_EVENT_VECTOR_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#ifdef ELOS_COMMON_FUNC_LINKER_WRAPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include "elos/event/event_vector.h"

MOCK_FUNC_PROTOTYPE(elosEventVectorNew, safuResultE_t, elosEventVector_t **eventVector, uint32_t elements)

MOCK_FUNC_PROTOTYPE(elosEventVectorInitialize, safuResultE_t, elosEventVector_t *eventVector, uint32_t elements)

MOCK_FUNC_PROTOTYPE(elosEventVectorToJsonArray, safuResultE_t, elosEventVector_t *eventVector,
                    json_object **jEventVector)

MOCK_FUNC_PROTOTYPE(elosEventVectorToJsonObject, safuResultE_t, elosEventVector_t *eventVector,
                    json_object **eventVecJobj)

MOCK_FUNC_PROTOTYPE(elosEventVectorFromJsonObject, safuResultE_t, json_object *eventVecJobj,
                    elosEventVector_t **eventVector)

MOCK_FUNC_PROTOTYPE(elosEventVectorFromJsonArray, safuResultE_t, json_object const *jEventArray,
                    elosEventVector_t **eventVector)

MOCK_FUNC_PROTOTYPE(elosEventVectorDeleteMembers, void, elosEventVector_t *eventVector)

MOCK_FUNC_PROTOTYPE(elosEventVectorDelete, void, elosEventVector_t *eventVector)

#endif /* __MOCK_EVENT_VECTOR_H__ */
