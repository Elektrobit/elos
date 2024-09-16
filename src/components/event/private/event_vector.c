// SPDX-License-Identifier: MIT

#include "elos/event/event_vector.h"

#include <json-c/json.h>  // json_object_put
#include <stdlib.h>

#include "safu/json.h"  // safuJsonAddObject
#include "safu/log.h"   // safuLog<...>

safuResultE_t elosEventVectorNew(elosEventVector_t **eventVector, uint32_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventVector != NULL) {
        elosEventVector_t *vector;

        vector = safuAllocMem(NULL, sizeof(elosEventVector_t));
        if (vector != NULL) {
            result = elosEventVectorInitialize(vector, elements);
            if (result != SAFU_RESULT_OK) {
                free(vector);
            } else {
                *eventVector = vector;
            }
        }
    }

    return result;
}

safuResultE_t elosEventVectorInitialize(elosEventVector_t *eventVector, uint32_t elements) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventVector != NULL) {
        int retVal;

        retVal = safuVecCreate(eventVector, elements, sizeof(elosEvent_t));
        if (retVal == 0) {
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

static inline safuResultE_t _eventVectorToJsonArrayConversionHelper(elosEventVector_t *eventVector, json_object *jArray,
                                                                    uint32_t idx) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    elosEvent_t *eventEntry = (elosEvent_t *)safuVecGet(eventVector, idx);
    if (eventEntry == NULL) {
        safuLogErrF("safuVecGet for index '%u' failed!", idx);
    } else {
        json_object *jEvent = json_object_new_object();
        if (jEvent == NULL) {
            safuLogErrF("json_object_new_object for index '%u' failed!", idx);
        } else {
            result = elosEventToJsonObject(jEvent, eventEntry);
            if (result != SAFU_RESULT_OK) {
                safuLogErrF("elosEventToJsonObject for index '%u' failed!", idx);
                json_object_put(jEvent);
            }
        }

        if (result == SAFU_RESULT_OK) {
            int retVal = json_object_array_put_idx(jArray, idx, jEvent);
            if (retVal < 0) {
                safuLogErrF("json_object_array_put_idx for index '%u' failed!", idx);
                json_object_put(jEvent);
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

safuResultE_t elosEventVectorToJsonArray(elosEventVector_t *eventVector, json_object **jEventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (jEventVector == NULL) {
        safuLogErr("Invalid parameter!");
    } else {
        json_object *jNewEventVector;
        uint32_t numEvents = 0;

        if (eventVector != NULL) {
            numEvents = eventVector->elementCount;
        }

        jNewEventVector = json_object_new_array_ext(numEvents);
        if (jNewEventVector == NULL) {
            safuLogErr("json_object_new_array_ext failed!");
        } else {
            result = SAFU_RESULT_OK;

            for (uint32_t idx = 0; idx < numEvents; idx++) {
                result = _eventVectorToJsonArrayConversionHelper(eventVector, jNewEventVector, idx);
                if (result != SAFU_RESULT_OK) {
                    break;
                }
            }
        }

        if (result == SAFU_RESULT_OK) {
            *jEventVector = jNewEventVector;
        } else {
            json_object_put(jNewEventVector);
        }
    }

    return result;
}

safuResultE_t elosEventVectorSerialize(elosEventVector_t *eventVector, char **sEventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (sEventVector == NULL) {
        safuLogErr("Invalid parameter!");
    } else {
        json_object *jEventVector = NULL;
        safuResultE_t resVal;

        resVal = elosEventVectorToJsonArray(eventVector, &jEventVector);
        if (resVal != SAFU_RESULT_OK) {
            safuLogErr("elosEventVectorToJsonArray failed!");
        } else {
            char const *tmpString;
            char *newString;

            tmpString = json_object_to_json_string_ext(jEventVector, JSON_C_TO_STRING_PLAIN);
            if (tmpString == NULL) {
                safuLogErr("json_object_to_json_string_ext failed!");
            } else {
                newString = strdup(tmpString);
                if (newString == NULL) {
                    safuLogErr("strdup failed!");
                } else {
                    *sEventVector = newString;
                    result = SAFU_RESULT_OK;
                }
            }
        }

        json_object_put(jEventVector);
    }

    return result;
}

static inline safuResultE_t _eventVectorFromJsonArrayConversionHelper(json_object const *jEventArray,
                                                                      elosEventVector_t *eventVector, uint32_t idx) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    struct json_object *jElement = json_object_array_get_idx(jEventArray, idx);
    if (jElement == NULL) {
        safuLogErrF("json_object_array_get_idx for index '%u' failed!", idx);
    } else {
        elosEvent_t *event = NULL;
        result = elosEventNew(&event);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("elosEventNew for index '%u' failed!", idx);
        } else {
            result = elosEventFromJsonObject(event, jElement);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErrF("elosEventFromJsonObject for index '%u' failed!", idx);
                elosEventDelete(event);
            } else {
                int retVal = safuVecPush(eventVector, (void *)event);
                if (retVal < 0) {
                    safuLogErrF("safuVecPush for index '%u' failed!", idx);
                    elosEventDelete(event);
                    result = SAFU_RESULT_FAILED;
                } else {
                    result = SAFU_RESULT_OK;
                    free(event);
                }
            }
        }
    }

    return result;
}

safuResultE_t elosEventVectorFromJsonArray(json_object const *jEventArray, elosEventVector_t **eventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if ((jEventArray == NULL) || (eventVector == NULL)) {
        safuLogErr("Invalid parameter!");
    } else {
        json_type jType;

        jType = json_object_get_type(jEventArray);
        if (jType != json_type_array) {
            safuLogErr("Json object is not an array!");
        } else {
            elosEventVector_t *newEventVector = NULL;
            size_t elements;

            elements = json_object_array_length(jEventArray);
            if (elements == 0) {
                result = SAFU_RESULT_OK;
            } else {
                result = elosEventVectorNew(&newEventVector, elements);
                if (result != SAFU_RESULT_OK) {
                    safuLogErr("elosEventVectorNew failed!");
                } else {
                    for (size_t idx = 0; idx < elements; idx += 1) {
                        result = _eventVectorFromJsonArrayConversionHelper(jEventArray, newEventVector, idx);
                        if (result != SAFU_RESULT_OK) {
                            break;
                        }
                    }
                }
            }

            if (result == SAFU_RESULT_OK) {
                *eventVector = newEventVector;
            } else {
                elosEventVectorDelete(newEventVector);
            }
        }
    }

    return result;
}

static safuResultE_t _createJsonObject(struct json_object **jobj) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    *jobj = json_object_new_object();
    if (!*jobj) {
        safuLogErr("json_object_new_object failed!");
    } else {
        result = SAFU_RESULT_OK;
    }
    return result;
}

safuResultE_t elosEventVectorToJsonObject(elosEventVector_t *eventVector, json_object **eventVecJobj) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    uint32_t numEvents = 0;

    if (eventVector == NULL || eventVecJobj == NULL) {
        safuLogErr("Invalid parameter!");
    } else {
        numEvents = eventVector->elementCount;
        result = _createJsonObject(eventVecJobj);
    }

    json_object *eventArray = NULL;
    if (result == SAFU_RESULT_OK) {
        eventArray = safuJsonAddNewArrayOfInitialSize(*eventVecJobj, "eventArray", numEvents);
        if (eventArray == NULL) {
            safuLogErr("Failed to create json array for events!");
            json_object_put(*eventVecJobj);
            *eventVecJobj = NULL;
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        for (uint32_t idx = 0; idx < numEvents; idx++) {
            elosEvent_t *eventEntry = (elosEvent_t *)safuVecGet(eventVector, idx);
            json_object *eventJobj = json_object_new_object();
            if (!eventJobj) {
                result = SAFU_RESULT_FAILED;
                safuLogErr("Failed to create json object for event entry!");
                json_object_put(*eventVecJobj);
                *eventVecJobj = NULL;
                break;
            } else {
                result = elosEventToJsonObject(eventJobj, eventEntry);
                if (result == SAFU_RESULT_FAILED) {
                    safuLogErr("Failed to fill json object with event entry!");
                    json_object_put(*eventVecJobj);
                    *eventVecJobj = NULL;
                    break;
                } else {
                    json_object_array_put_idx(eventArray, idx, eventJobj);
                }
            }
        }
    }

    return result;
}

safuResultE_t elosEventVectorFromJsonObject(json_object *eventVecJobj, elosEventVector_t **eventVector) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (eventVecJobj == NULL || eventVector == NULL) {
        safuLogErr("Invalid parameter!");
    } else {
        size_t numEvents = 0;
        json_object *eventArray = safuJsonGetArray(eventVecJobj, "eventArray", 0, &numEvents);
        if (!eventArray) {
            safuLogErr("Failed to get event array!");
        } else if (numEvents > 0) {
            elosEventVector_t *tmpEventVector = NULL;
            result = elosEventVectorNew(&tmpEventVector, numEvents);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("Failed to create event vector!");
            } else {
                for (uint32_t idx = 0; idx < numEvents; idx++) {
                    json_object *eventJobj = json_object_array_get_idx(eventArray, idx);
                    elosEvent_t *eventEntry = NULL;
                    result = elosEventNew(&eventEntry);
                    if (result == SAFU_RESULT_FAILED) {
                        safuLogDebugF("Failed to create event entry at idx %d!", idx + 1);
                        break;
                    } else {
                        result = elosEventFromJsonObject(eventEntry, eventJobj);
                        if (result == SAFU_RESULT_FAILED) {
                            safuLogDebugF("Failed to fill event entry at idx %d!", idx + 1);
                            elosEventDelete(eventEntry);
                            break;
                        } else {
                            safuVecPush(tmpEventVector, eventEntry);  // copies event and free's its members
                            free(eventEntry);
                        }
                    }
                }
                if (tmpEventVector->elementCount < numEvents) {
                    safuLogErr("Failed to read events from json object!");
                    elosEventVectorDelete(tmpEventVector);
                    result = SAFU_RESULT_FAILED;
                } else {
                    *eventVector = tmpEventVector;
                    result = SAFU_RESULT_OK;
                }
            }
        } else {
            *eventVector = NULL;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}

safuResultE_t elosEventVectorPush(elosEventVector_t *eventVector, elosEvent_t const *event) {
    return safuVecPush(eventVector, event);
}

safuResultE_t elosEventVectorPushDeepCopy(elosEventVector_t *eventVector, elosEvent_t const *event) {
    elosEvent_t newEvent = {0};
    safuResultE_t result = elosEventDeepCopy(&newEvent, event);
    if (result == SAFU_RESULT_OK) {
        result = safuVecPush(eventVector, &newEvent);
    }
    return result;
}

void elosEventVectorDeleteMembers(elosEventVector_t *eventVector) {
    if (eventVector != NULL) {
        for (uint32_t idx = 0; idx < eventVector->elementCount; idx++) {
            elosEvent_t *event = (elosEvent_t *)safuVecGet(eventVector, idx);
            elosEventDeleteMembers(event);
        }
        safuVecFree(eventVector);
    }
}

void elosEventVectorDelete(elosEventVector_t *eventVector) {
    if (eventVector != NULL) {
        elosEventVectorDeleteMembers(eventVector);
        free(eventVector);
    }
}
