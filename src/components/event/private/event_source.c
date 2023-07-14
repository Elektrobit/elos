// SPDX-License-Identifier: MIT

#include "elos/event/event_source.h"

#include <json-c/json_tokener.h> /* json_tokener_parse */
#include <stdlib.h>              /* calloc, free */
#include <string.h>              /* strdup */

#include "safu/common.h"
#include "safu/json.h"
#include "safu/log.h"

safuResultE_t elosEventSourceNew(elosEventSource_t **eventSource) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    *eventSource = calloc(1, sizeof(elosEventSource_t));
    if (*eventSource) {
        result = SAFU_RESULT_OK;
    }
    return result;
}

void elosEventSourceDeleteMembers(elosEventSource_t *eventSource) {
    if (eventSource != NULL) {
        free(eventSource->appName);
        free(eventSource->fileName);
    }
}

void elosEventSourceDelete(elosEventSource_t *eventSource) {
    if (eventSource != NULL) {
        elosEventSourceDeleteMembers(eventSource);
        free(eventSource);
    }
}

safuResultE_t elosEventSourceToJsonObject(json_object *to, const elosEventSource_t *from) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (!to || !from) {
        safuLogErr("parameter is NULL!");
        result = SAFU_RESULT_FAILED;
    } else {
        if (from->appName != NULL) {
            if (!safuJsonAddNewString(to, "appName", from->appName)) {
                safuLogErr("safuJsonAddNewString failed!");
                result = SAFU_RESULT_FAILED;
            }
        }

        if (from->fileName != NULL) {
            if (!safuJsonAddNewString(to, "fileName", from->fileName)) {
                safuLogErr("safuJsonAddNewString failed!");
                result = SAFU_RESULT_FAILED;
            }
        }

        if (from->pid != 0) {
            if (!safuJsonAddNewInt(to, "pid", from->pid)) {
                safuLogErr("safuJsonAddNewInt failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

safuResultE_t elosEventSourceSerialize(char **toJsonString, elosEventSource_t *from) {
    safuResultE_t result = SAFU_RESULT_OK;
    json_object *jobj = NULL;

    if (!from) {
        safuLogErr("parameter 'from' is NULL!");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        jobj = json_object_new_object();
        if (!jobj) {
            safuLogErr("conversion to json object failed");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = elosEventSourceToJsonObject(jobj, from);
    }

    if (result == SAFU_RESULT_OK) {
        const char *tmp = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY);
        if (!tmp) {
            safuLogErr("json_object_to_json_string_ext failed");
            result = SAFU_RESULT_FAILED;
        } else {
            *toJsonString = strdup(tmp);
            if (!*toJsonString) {
                safuLogErr("allocating string failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (jobj) {
        json_object_put(jobj);
    }

    return result;
}

safuResultE_t elosEventSourceFromJsonObject(elosEventSource_t *to, const json_object *from) {
    safuResultE_t result = SAFU_RESULT_OK;
    const char *tmp = NULL;

    if (!to || !from) {
        safuLogErr("parameter is NULL!");
        result = SAFU_RESULT_FAILED;
    } else {
        if (json_object_object_get(from, "appName") != NULL) {
            if (safuJsonGetString(from, "appName", 0, &tmp) != 0) {
                safuLogErr("safuJsonGetString failed!");
                result = SAFU_RESULT_FAILED;
            } else {
                if (to->appName) {
                    free(to->appName);
                }
                to->appName = strdup(tmp);
                if (!to->appName) {
                    safuLogErr("allocating appName failed!");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }

        if (json_object_object_get(from, "fileName") != NULL) {
            if (safuJsonGetString(from, "fileName", 0, &tmp) != 0) {
                safuLogErr("safuJsonGetString failed!");
                result = SAFU_RESULT_FAILED;
            } else {
                if (to->fileName) {
                    free(to->fileName);
                }
                to->fileName = strdup(tmp);
                if (!to->fileName) {
                    safuLogErr("allocating fileName failed!");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }

        if (json_object_object_get(from, "pid") != NULL) {
            if (safuJsonGetInt32(from, "pid", 0, &to->pid) != 0) {
                safuLogErr("safuJsonGetInt32 failed!");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

safuResultE_t elosEventSourceDeserialize(elosEventSource_t *to, const char *fromJsonString) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (!to || !fromJsonString) {
        safuLogErr("parameter is NULL!");
        result = SAFU_RESULT_FAILED;
    } else {
        json_object *jobj = json_tokener_parse(fromJsonString);
        if (!jobj) {
            safuLogErr("json_tokener_parse failed!");
            result = SAFU_RESULT_FAILED;
        } else {
            result = elosEventSourceFromJsonObject(to, jobj);
            json_object_put(jobj);
        }
    }
    return result;
}

safuResultE_t elosEventSourceDeepCopy(elosEventSource_t *to, const elosEventSource_t *from) {
    safuResultE_t result = SAFU_RESULT_OK;

    if ((to == NULL) || (from == NULL)) {
        safuLogErr("parameter is NULL!");
        result = SAFU_RESULT_FAILED;
    } else {
        to->pid = from->pid;

        if (from->appName != NULL) {
            char *appName = strdup(from->appName);
            if (appName == NULL) {
                safuLogErr("strdup from->appName failed!");
                result = SAFU_RESULT_FAILED;
            } else {
                to->appName = appName;
            }
        } else {
            to->appName = NULL;
        }

        if (from->fileName != NULL) {
            char *fileName = strdup(from->fileName);
            if (fileName == NULL) {
                safuLogErr("strdup from->fileName failed!");
                result = SAFU_RESULT_FAILED;
            } else {
                to->fileName = fileName;
            }
        } else {
            to->fileName = NULL;
        }
    }
    return result;
}
