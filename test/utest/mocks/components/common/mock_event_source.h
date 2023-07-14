// SPDX-License-Identifier: MIT

#ifndef __MOCK_COMMON_EVENT_SOURCE_H__
#define __MOCK_COMMON_EVENT_SOURCE_H__

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#ifdef ELOS_COMMON_FUNC_LINKER_WRAPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif
#include <json-c/json_types.h>

#include "elos/event/event_source.h"

MOCK_FUNC_PROTOTYPE(elosEventSourceNew, safuResultE_t, elosEventSource_t **eventSource)

MOCK_FUNC_PROTOTYPE(elosEventSourceDeleteMembers, void, elosEventSource_t *eventSource)

MOCK_FUNC_PROTOTYPE(elosEventSourceDelete, void, elosEventSource_t *eventSource)

MOCK_FUNC_PROTOTYPE(elosEventSourceSerialize, safuResultE_t, char **toJsonString, elosEventSource_t *from)

MOCK_FUNC_PROTOTYPE(elosEventSourceDeserialize, safuResultE_t, elosEventSource_t *to, const char *fromJsonString)

MOCK_FUNC_PROTOTYPE(elosEventSourceDeepCopy, safuResultE_t, elosEventSource_t *to, const elosEventSource_t *from)

MOCK_FUNC_PROTOTYPE(elosEventSourceToJsonObject, safuResultE_t, json_object *to, const elosEventSource_t *from)

MOCK_FUNC_PROTOTYPE(elosEventSourceFromJsonObject, safuResultE_t, elosEventSource_t *to, const json_object *from)

#endif /* __MOCK_COMMON_EVENT_SOURCE_H__ */
