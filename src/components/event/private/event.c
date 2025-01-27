// SPDX-License-Identifier: MIT

#include "elos/event/event.h"

#include <json-c/json_tokener.h> /* json_tokener_parse */
#include <safu/common.h>
#include <safu/json.h>
#include <safu/log.h>
#include <stdlib.h>

static safuResultE_t elosStrdup(char **to, const char *from) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (from != NULL) {
        char *str = strdup(from);
        if (str == NULL) {
            result = SAFU_RESULT_FAILED;
        } else {
            *to = str;
        }
    } else {
        *to = NULL;
    }
    return result;
}

safuResultE_t elosEventClone(elosEvent_t **to, elosEvent_t const *from) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((to == NULL) || (from == NULL)) {
        safuLogErr("invalid parameter");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventNew(to);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventNew failed!");
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventDeepCopy(*to, from);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventDeepCopy failed!");
            elosEventDelete(*to);
        }
    }

    return result;
}

safuResultE_t elosEventDeepCopy(elosEvent_t *to, const elosEvent_t *from) {
    safuResultE_t result = SAFU_RESULT_OK;
    if ((to == NULL) || (from == NULL)) {
        safuLogErr("invalid parameter");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        // This will copy more than we need, but is still much faster than variable based copy due to optimizations
        memcpy(to, from, sizeof(elosEvent_t));
    }

    if (result == SAFU_RESULT_OK) {
        result = elosStrdup(&to->hardwareid, from->hardwareid);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("strdup(from->hardwareId) failed!");
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosStrdup(&to->payload, from->payload);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("strdup(from->payload) failed!");
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventSourceDeepCopy(&to->source, &from->source);
    }

    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosEventSourceDeepCopy failed!");
    }

    return result;
}

safuResultE_t elosEventNew(elosEvent_t **event) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    *event = safuAllocMem(NULL, sizeof(elosEvent_t));
    if (*event) {
        result = elosEventInitialize(*event);
    }
    return result;
}

safuResultE_t elosEventInitialize(elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    if (event != NULL) {
        memset(event, '\0', sizeof(elosEvent_t));
        result = SAFU_RESULT_OK;
    }
    return result;
}

safuResultE_t elosEventDeleteMembers(elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    if (event != NULL) {
        elosEventSourceDeleteMembers(&event->source);
        free(event->hardwareid);
        free(event->payload);
        result = SAFU_RESULT_OK;
    }
    return result;
}

safuResultE_t elosEventDelete(elosEvent_t *event) {
    safuResultE_t result = elosEventDeleteMembers(event);
    free(event);
    return result;
}

safuResultE_t elosEventSerialize(char **toJsonString, const elosEvent_t *from) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((toJsonString == NULL) || (from == NULL)) {
        safuLogErr("invalid parameter");
        result = SAFU_RESULT_FAILED;
    }

    json_object *jobj = NULL;
    if (result == SAFU_RESULT_OK) {
        jobj = json_object_new_object();
        if (!jobj) {
            safuLogErr("json_object_new_object failed!");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventToJsonObject(jobj, from);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventToJsonObject failed!");
        }
    }

    const char *tmpJsonStr = NULL;
    if (result == SAFU_RESULT_OK) {
        tmpJsonStr = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN);
        if (tmpJsonStr == NULL) {
            safuLogErr("json_object_to_json_string_ext failed!");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        *toJsonString = strdup(tmpJsonStr);
        if (*toJsonString == NULL) {
            safuLogErr("strdup failed!");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (jobj) {
        json_object_put(jobj);
    }

    return result;
}

safuResultE_t elosEventDeserialize(elosEvent_t *to, const char *fromJsonString) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((to == NULL) || (fromJsonString == NULL)) {
        safuLogErr("invalid parameter");
        result = SAFU_RESULT_FAILED;
    }

    json_object *jobj = NULL;
    if (result == SAFU_RESULT_OK) {
        jobj = json_tokener_parse(fromJsonString);
        if (!jobj) {
            safuLogErr("json_object_new_object failed!");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventFromJsonObject(to, jobj);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosEventFromJsonObject failed!");
        }
    }

    if (jobj != NULL) {
        json_object_put(jobj);
    }

    return result;
}

safuResultE_t elosEventFromJsonObject(elosEvent_t *to, const json_object *from) {
    safuResultE_t result = SAFU_RESULT_OK;
    const char *tmp;

    if (!to || !from) {
        safuLogErr("passed empty parameter!");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        if (json_object_object_get(from, "date") != NULL) {
            if (safuJsonGetTimestamp(from, "date", 0, &to->date)) {
                safuLogErr("event date deserialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        json_object *jsource = json_object_object_get(from, "source");
        if (jsource != NULL) {
            result = elosEventSourceFromJsonObject(&to->source, jsource);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("deserializing event source failed!");
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        int32_t sevType;
        if (json_object_object_get(from, "severity") != NULL) {
            if (safuJsonGetInt32(from, "severity", 0, &sevType)) {
                safuLogErr("event severity deserialize failed!");
                result = SAFU_RESULT_FAILED;
            } else {
                to->severity = sevType;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (json_object_object_get(from, "hardwareid") != NULL) {
            if (safuJsonGetString(from, "hardwareid", 0, &tmp)) {
                safuLogErr("event hardwareid deserialize failed!");
                result = SAFU_RESULT_FAILED;
            } else {
                to->hardwareid = strdup(tmp);
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (json_object_object_get(from, "classification") != NULL) {
            if (safuJsonGetUint64(from, "classification", 0, &to->classification)) {
                safuLogErr("event classification deserialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (json_object_object_get(from, "messageCode") != NULL) {
            if (safuJsonGetInt32(from, "messageCode", 0, (int *)&to->messageCode)) {
                safuLogErr("event messageCode deserialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (json_object_object_get(from, "payload") != NULL) {
            if (safuJsonGetString(from, "payload", 0, &tmp)) {
                safuLogErr("event payload deserialize failed!");
                result = SAFU_RESULT_FAILED;
            } else {
                to->payload = strdup(tmp);
            }
        }
    }

    return result;
}

safuResultE_t elosEventToJsonObject(json_object *to, const elosEvent_t *from) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (!to || !from) {
        safuLogErr("passed empty parameter!");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        if (from->date.tv_sec != 0 || from->date.tv_nsec != 0) {
            if (!safuJsonAddNewTimestamp(to, "date", &from->date)) {
                safuLogErr("event date serialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    json_object *jsource = NULL;
    if (result == SAFU_RESULT_OK) {
        jsource = json_object_new_object();
        if (!jsource) {
            safuLogErr("creating new event source json object failed!");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventSourceToJsonObject(jsource, &from->source);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("filling event source json object failed!");
            json_object_put(jsource);
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (safuJsonAddObject(to, "source", jsource) < 0) {
            safuLogErr("adding event source json object to event object failed!");
            json_object_put(jsource);
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (from->severity != ELOS_SEVERITY_OFF) {
            if (!safuJsonAddNewInt(to, "severity", from->severity)) {
                safuLogErr("event severity serialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (from->hardwareid != NULL) {
            if (!safuJsonAddNewString(to, "hardwareid", from->hardwareid)) {
                safuLogErr("event hardwareid serialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (from->classification != 0) {
            if (!safuJsonAddNewUint64(to, "classification", from->classification)) {
                safuLogErr("event classification serialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (from->messageCode != 0) {
            if (!safuJsonAddNewInt(to, "messageCode", from->messageCode)) {
                safuLogErr("event messageCode serialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        if (from->payload != NULL) {
            if (!safuJsonAddNewString(to, "payload", from->payload)) {
                safuLogErr("event payload serialize failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

bool elosEventIsDateEmpty(const elosEvent_t *const event) {
    return event->date.tv_sec == 0 && event->date.tv_nsec == 0;
}
