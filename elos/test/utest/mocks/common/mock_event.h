// SPDX-License-Identifier: MIT

#ifndef __MOCK_EVENT_H__
#define __MOCK_EVENT_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#ifdef ELOS_COMMON_FUNC_LINKER_WRAPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif
#include <json-c/json_types.h>

#include "elos/event/event.h"

MOCK_FUNC_PROTOTYPE(elosEventSerialize, safuResultE_t, char **toJsonString, const elosEvent_t *from)

MOCK_FUNC_PROTOTYPE(elosEventDeepCopy, safuResultE_t, elosEvent_t *to, const elosEvent_t *from)

MOCK_FUNC_PROTOTYPE(elosEventNew, safuResultE_t, elosEvent_t **event)

MOCK_FUNC_PROTOTYPE(elosEventDeleteMembers, safuResultE_t, elosEvent_t *event)

MOCK_FUNC_PROTOTYPE(elosEventDelete, safuResultE_t, elosEvent_t *event)

MOCK_FUNC_PROTOTYPE(elosEventDeserialize, safuResultE_t, elosEvent_t *to, const char *fromJsonString)

MOCK_FUNC_PROTOTYPE(elosEventFromJsonObject, safuResultE_t, elosEvent_t *to, const json_object *from)

MOCK_FUNC_PROTOTYPE(elosEventToJsonObject, safuResultE_t, json_object *to, const elosEvent_t *from)

MOCK_FUNC_PROTOTYPE(elosEventIsDateEmpty, bool, const elosEvent_t *const event)

#endif /* __MOCK_EVENT_H__ */
