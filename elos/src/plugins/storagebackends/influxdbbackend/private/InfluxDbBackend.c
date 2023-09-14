// SPDX-License-Identifier: MIT
#include <curl/curl.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <safu/common.h>
#include <safu/json.h>
#include <safu/log.h>
#include <safu/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "InfluxDb.h"
#include "elos/event/event_vector.h"
#include "elos/eventfilter/eventfilter.h"
#include "elos/eventlogging/StorageBackend.h"
#include "elos/rpnfilter/rpnfilter_types.h"
#include "samconf/json_backend.h"

#define BACKEND_NAME "InfluxDB"

struct callbackStorage {
    char *response;
    size_t size;
};

static inline size_t elosWriteCallback(void *data, size_t size, size_t dataSize, void *userData) {
    if (userData == NULL) {
        return dataSize;
    }
    size_t realsize = size * dataSize;
    struct callbackStorage *storage = (struct callbackStorage *)userData;

    char *newData = safuAllocMem(storage->response, storage->size + realsize + 1);
    if (newData == NULL) {
        safuLogErr("safuAllocMem");
        return 0;
    }

    storage->response = newData;
    memcpy(&(storage->response[storage->size]), data, realsize);
    storage->size += realsize;
    storage->response[storage->size] = 0;

    return realsize;
}

#define COND_STRING(name, size, cond, val1, val2) \
    char name[size];                              \
    if (cond) {                                   \
        strcpy(name, val1);                       \
    } else {                                      \
        strcpy(name, val2);                       \
    }
static inline safuResultE_t _request(elosInfluxDbBackend_t *influxBackend, bool write, char *cmd,
                                     struct callbackStorage *output) {
    bool cleanupRequest = true;
    safuResultE_t result = SAFU_RESULT_OK;
    struct curl_slist *header = NULL;
    size_t authLen = strlen("u=%s&p=%s") + strlen(influxBackend->user) + strlen(influxBackend->pw);
    char authUrl[authLen];
    COND_STRING(type, 13, write, "write", "query")
    COND_STRING(timeFormat, 10, write, "precision", "epoch")
    size_t urlLen = strlen("http://%s/%s?org=%s&db=%s&%s=ns&%s") +  strlen(influxBackend->host) + strlen(type) + strlen(influxBackend->orgId) + strlen(influxBackend->db) + strlen(timeFormat) + authLen + strlen(cmd) + 1;
    char url[urlLen];
    char tempUrl[urlLen];
    int ret = 0;
    CURLcode res;

    CURL *request = curl_easy_init();
    if (!request) {
        cleanupRequest = false;
        result = SAFU_RESULT_FAILED;
        safuLogErr("Failed to initialize curl request");
    }

    if (result == SAFU_RESULT_OK) {
        header = curl_slist_append(header, ACCEPT_APP_JSON);
        if (header == NULL) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to initialize curl authorization header");
        }
    }

    if (result == SAFU_RESULT_OK) {
        header = curl_slist_append(header, write ? CONTENT_TYPE_TEXT_PLAIN : CONTENT_TYPE_APP_JSON);
        if (header == NULL) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to initialize curl content-type header");
        }
    }

    if (result == SAFU_RESULT_OK) {
        res = curl_easy_setopt(request, CURLOPT_HTTPHEADER, header);
        if (res != CURLE_OK) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to initialize curl header");
        }
    }

    if (result == SAFU_RESULT_OK) {
        ret = sprintf(authUrl, "u=%s&p=%s", influxBackend->user, influxBackend->pw);
        if (ret <= 0) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to initialize curl accept header");
        }
    }

    if (result == SAFU_RESULT_OK) {
        ret = sprintf(url, "http://%s/%s?org=%s&db=%s&%s=ns&%s", influxBackend->host, type, influxBackend->orgId,
                      influxBackend->db, timeFormat, authUrl);
        if (ret <= 0) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to initialize url");
        }
    }

    if (result == SAFU_RESULT_OK && !write) {
        ret = sprintf(tempUrl, "%s&%s", url, cmd);
        if (ret <= 0) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to add query to url");
        }
    }

    if (result == SAFU_RESULT_OK && !write) {
        strcpy(url, tempUrl);
        if (url == NULL) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("strcpy");
        }
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("Sending request to url %s", url);
        res = curl_easy_setopt(request, CURLOPT_URL, url);
        if (res != CURLE_OK) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to set request url");
        }
    }

    if (result == SAFU_RESULT_OK && write) {
        res = curl_easy_setopt(request, CURLOPT_POSTFIELDS, cmd);
        if (res != CURLE_OK) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to set post data");
        }
    }

    if (result == SAFU_RESULT_OK && !write) {
        res = curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, elosWriteCallback);
        if (res != CURLE_OK) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to set set callback");
        }
    }

    if (result == SAFU_RESULT_OK && !write) {
        res = curl_easy_setopt(request, CURLOPT_WRITEDATA, (void *)output);
        if (res != CURLE_OK) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Failed to set callback buffer");
        }
    }

    if (result == SAFU_RESULT_OK) {
        char buf[500];
        curl_easy_setopt(request, CURLOPT_ERRORBUFFER, buf);
        CURLcode curlRet = curl_easy_perform(request);
        if (curlRet != CURLE_OK) {
            result = SAFU_RESULT_FAILED;
            safuLogErrF("Failed to connect to influx db via curl: %s: %s", curl_easy_strerror(curlRet), buf);
        }
    }

    if (cleanupRequest) {
        curl_easy_cleanup(request);
        curl_slist_free_all(header);
    }

    return result;
}

safuResultE_t elosInfluxDbBackendStart(elosStorageBackend_t *backend) {
    elosInfluxDbBackend_t *influxBackend = backend->backendData;
    return _request(influxBackend, false, "q=Select%20*%20FROM%20event", NULL);
}

safuResultE_t elosInfluxDbBackendShutdown(elosStorageBackend_t *backend) {
    safuLogDebugF("shutdown backend %s", backend->name);
    free(backend->backendData);
    free(backend);
    return SAFU_RESULT_OK;
}

size_t _strlen_null(const char *str) {
    if (str == NULL) {
        return strlen("(NULL)");
    } else {
        return strlen(str);
    }
}

#define protocolLine                                                       \
    "event,sourceAppName=%s,sourceFileName=%s,sourcePid=%i,hardwareid=%s " \
    "severity=%i,classification=%lu,messageCode=%i,payload=\"%s\" %lu%lu"
size_t _predictMessageSize(const elosEvent_t *event) {
    size_t out = strlen(protocolLine);
    out += _strlen_null(event->source.fileName);
    out += _strlen_null(event->source.appName);
    out += DECIMAL_DIGITS_BOUND(event->source.pid);
    out += _strlen_null(event->hardwareid);
    out += DECIMAL_DIGITS_BOUND(event->severity);
    out += DECIMAL_DIGITS_BOUND(event->classification);
    out += DECIMAL_DIGITS_BOUND(event->messageCode);
    out += _strlen_null(event->payload);
    out += DECIMAL_DIGITS_BOUND(event->date.tv_sec);
    out += DECIMAL_DIGITS_BOUND(event->date.tv_nsec);
    return out;
    /*return strlen(protocolLine) + strlen(event->source.fileName) + strlen(event->source.appName) +
           DECIMAL_DIGITS_BOUND(event->source.pid) + strlen(event->hardwareid) + DECIMAL_DIGITS_BOUND(event->severity) +
           DECIMAL_DIGITS_BOUND(event->classification) + DECIMAL_DIGITS_BOUND(event->messageCode) +
           strlen(event->payload) + DECIMAL_DIGITS_BOUND(event->date.tv_sec) +
           DECIMAL_DIGITS_BOUND(event->date.tv_nsec);*/
}

static inline char *_eventToLineProtocol(const elosEvent_t *event) {
    char *out = safuAllocMem(NULL, sizeof(char) * _predictMessageSize(event));
    int ret = sprintf(out, protocolLine, event->source.appName, event->source.fileName, event->source.pid,
                      event->hardwareid, event->severity, event->classification, event->messageCode, event->payload,
                      event->date.tv_sec, event->date.tv_nsec);
    if (ret <= 0) {
        safuLogErr("Failed to write event using line protocol");
        out = "";
    }
    out = safuAllocMem(out, sizeof(char) * (strlen(out) + 1));
    return out;
}

safuResultE_t elosInfluxDbBackendPersist(elosStorageBackend_t *backend, const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosInfluxDbBackend_t *influxBackend = backend->backendData;

    char *lineProtocolString = _eventToLineProtocol(event);
    if (strcmp(lineProtocolString, "") == 0) {
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        safuLogDebugF("persist to %s : %s", backend->name, lineProtocolString);
        result = _request(influxBackend, true, lineProtocolString, NULL);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("Write Request failed");
        }
    }

    free(lineProtocolString);

    if (result != SAFU_RESULT_OK) {
        safuLogErr("Failed to persist event");
    }

    return result;
}

static inline void _timespecFromEpochNsec(uint64_t epochNsec, struct timespec *date) {
    date->tv_sec = epochNsec / 1000000000;
    date->tv_nsec = epochNsec - (date->tv_sec * 1000000000);
}

static inline bool _fillEventStruct(elosEvent_t *event, json_object *value, json_object *columns, int idx) {
    bool filled = false;
    char *key;
    char *buf = NULL;
    uint64_t epochNsec;
    int ret = safuJsonGetString(columns, NULL, idx, (const char **)&key);
    if (ret == 0) {
        if (strcmp(key, "time") == 0) {
            ret = safuJsonGetUint64(value, NULL, idx, &epochNsec);
            if (ret == 0) {
                _timespecFromEpochNsec(epochNsec, &event->date);
                filled = true;
            }
        } else if (strcmp(key, "classification") == 0) {
            ret = safuJsonGetUint64(value, NULL, idx, &event->classification);
            if (ret == 0) {
                filled = true;
            }
        } else if (strcmp(key, "hardwareid") == 0) {
            ret = safuJsonGetString(value, NULL, idx, (const char **)&buf);
            if (ret == 0) {
                if (strcmp(buf, "(null)") != 0) {
                    event->hardwareid = strdup(buf);
                }
                filled = true;
            }
        } else if (strcmp(key, "severity") == 0) {
            ret = safuJsonGetInt32(value, NULL, idx, (int *)&event->severity);
            if (ret == 0) {
                filled = true;
            }
        } else if (strcmp(key, "sourceAppName") == 0) {
            ret = safuJsonGetString(value, NULL, idx, (const char **)&buf);
            if (ret == 0) {
                if (strcmp(buf, "(null)") != 0) {
                    event->source.appName = strdup(buf);
                }
                filled = true;
            }
        } else if (strcmp(key, "sourceFileName") == 0) {
            ret = safuJsonGetString(value, NULL, idx, (const char **)&buf);
            if (ret == 0) {
                if (strcmp(buf, "(null)") != 0) {
                    event->source.fileName = strdup(buf);
                }
                filled = true;
            }
        } else if (strcmp(key, "sourcePid") == 0) {
            ret = safuJsonGetInt32(value, NULL, idx, &event->source.pid);
            if (ret == 0) {
                filled = true;
            }
        } else if (strcmp(key, "messageCode") == 0) {
            ret = safuJsonGetInt32(value, NULL, idx, (int *)&event->messageCode);
            if (ret == 0) {
                filled = true;
            }
        } else if (strcmp(key, "payload") == 0) {
            ret = safuJsonGetString(value, NULL, idx, (const char **)&buf);
            if (ret == 0) {
                if (strcmp(buf, "(null)") != 0) {
                    event->payload = strdup(buf);
                }
                filled = true;
            }
        } else {
            safuLogErrF("Invalid key name in data point: '%s' at idx %i", key, idx);
        }
        if (!filled) {
            safuLogErrF("Failed to parse data point at idx %i", idx);
        }
    } else {
        safuLogErrF("could not retrive key at idx %i", idx);
    }

    return filled;
}

static inline safuResultE_t _influxJsonToEvents(json_object *dataPoint, elosEventVector_t *events) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEvent_t *event = NULL;
    size_t lenColumns, lenValues, lenValue;
    unsigned int i, j;
    int ret;
    json_object *columns = NULL;
    json_object *values = NULL;
    json_object *value = NULL;
    json_object *seriesEntry = NULL;
    json_object *series = safuJsonGetArray(dataPoint, "series", 0, NULL);
    if (series == NULL) {
        result = SAFU_RESULT_FAILED;
        safuLogErr("Invalid Data point, failed to extract series.");
    }

    if (result != SAFU_RESULT_FAILED) {
        seriesEntry = safuJsonGetObject(series, NULL, 0);
        if (seriesEntry == NULL) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Invalid Data point, failed to extract series entry.");
        }
    }

    if (result != SAFU_RESULT_FAILED) {
        columns = safuJsonGetArray(seriesEntry, "columns", 0, &lenColumns);
        if (columns == NULL || lenColumns <= 0) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Invalid Data point, failed to extract columns.");
        }
    }

    if (result != SAFU_RESULT_FAILED) {
        values = safuJsonGetArray(seriesEntry, "values", 0, &lenValues);
        if (values == NULL || lenValues <= 0) {
            result = SAFU_RESULT_FAILED;
            safuLogErr("Invalid Data point, failed to extract values.");
        }
    }

    if (result != SAFU_RESULT_FAILED) {
        for (i = 0; i < lenValues; i++) {
            value = safuJsonGetArray(values, NULL, i, &lenValue);
            if (value == NULL) {
                result = SAFU_RESULT_FAILED;
                safuLogErr("Invalid Data point, failed to extract value.");
            }
            if (result != SAFU_RESULT_FAILED) {
                if (lenValue != lenColumns) {
                    result = SAFU_RESULT_FAILED;
                    safuLogErr("Invalid Data point, value and columns are not equal size.");
                }
            }

            if (result != SAFU_RESULT_FAILED) {
                result = elosEventNew(&event);
                if (event == NULL) {
                    result = SAFU_RESULT_FAILED;
                    safuLogErr("Failed to allocate event");
                }
            }

            if (result != SAFU_RESULT_FAILED) {
                for (j = 0; j < lenValue; j++) {
                    if (!_fillEventStruct(event, value, columns, j)) {
                        result = SAFU_RESULT_FAILED;
                        safuLogErr("Invalid Data Point, failed to extract event");
                        break;
                    }
                }
            }

            if (result != SAFU_RESULT_FAILED) {
                ret = safuVecPush(events, event);
                if (ret != 0) {
                    result = SAFU_RESULT_FAILED;
                    safuLogErr("Failed to push event to vector");
                }
                free(event);
            }
        }
    }

    return result;
}

safuResultE_t elosInfluxDbBackendFindEvents(elosStorageBackend_t *backend, elosEventFilter_t *filter,
                                            safuVec_t *events) {
    int i;
    size_t arrayLength = 0;
    struct callbackStorage buf = {0};
    json_object *root = NULL;
    json_object *result = NULL;
    json_object *results = NULL;
    elosEvent_t *event = NULL;
    safuResultE_t res = SAFU_RESULT_OK;
    elosRpnFilterResultE_t filterResult;
    elosInfluxDbBackend_t *influxBackend = backend->backendData;

    if (res == SAFU_RESULT_OK) {
        res = _request(influxBackend, false, "q=Select%20*%20FROM%20event", &buf);

        if (strcmp(buf.response, "") == 0 || res != SAFU_RESULT_OK) {
            res = SAFU_RESULT_FAILED;
            safuLogErr("Failed to extract series 'event' from bucket 'elosd'");
        }
        if (res == SAFU_RESULT_OK) {
            safuLogDebugF("Parsing received json: %s", buf.response);
            root = json_tokener_parse(buf.response);
            if (root == NULL) {
                res = SAFU_RESULT_FAILED;
                safuLogErrF("Failed to parse influxdb json output: %s", buf.response);
            }
        }
    }

    if (res == SAFU_RESULT_OK) {
        results = safuJsonGetArray(root, "results", 0, &arrayLength);
        if (results == NULL || arrayLength != 1) {
            res = SAFU_RESULT_FAILED;
            safuLogErrF("Invalid json from influx db: %s", buf.response);
        }
    }

    if (res == SAFU_RESULT_OK) {
        result = safuJsonGetObject(results, NULL, 0);
        if (result == NULL) {
            res = SAFU_RESULT_FAILED;
            safuLogErrF("Invalid json from influx db, failed to extract result: %s", buf.response);
        }
    }

    if (res == SAFU_RESULT_OK) {
        res = _influxJsonToEvents(result, events);
    }

    if (res == SAFU_RESULT_OK) {
        for (i = events->elementCount - 1; i >= 0; i--) {
            event = safuVecGet(events, i);
            if (event != NULL) {
                filterResult = elosEventFilterExecute(filter, NULL, event);
                if (filterResult == RPNFILTER_RESULT_MATCH) {
                    safuLogDebug("Append event: Match");
                } else if (filterResult == RPNFILTER_RESULT_NO_MATCH) {
                    safuLogDebug("Dont append event: No match");
                    elosEventDeleteMembers(event);
                    safuVecRemove(events, i);
                } else {
                    safuLogErr("elosEventFilterExecute failed");
                    safuLogDebugF("Dont append Event: RPN filter result is: %d", filterResult);
                    elosEventDeleteMembers(event);
                    safuVecRemove(events, i);
                    res = SAFU_RESULT_FAILED;
                }
            } else {
                res = SAFU_RESULT_FAILED;
                safuLogErr("Failed to fetch event from list");
            }
        }
    }

    if (buf.response != NULL) {
        free(buf.response);
    }
    if (root != NULL) {
        json_object_put(root);
    }

    return res;
}

safuResultE_t elosInfluxDbBackendNew(elosStorageBackend_t **backend) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosStorageBackend_t *newBackend = NULL;

    newBackend = safuAllocMem(NULL, sizeof(elosStorageBackend_t));
    if (newBackend == NULL) {
        safuLogErr("safuAllocMem failed");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        memset(newBackend, 0, sizeof(elosStorageBackend_t));
        newBackend->backendData = safuAllocMem(NULL, sizeof(elosInfluxDbBackend_t));
        if (newBackend->backendData == NULL) {
            safuLogCrit("safuAllocMem failed");
            free(newBackend);
            result = SAFU_RESULT_FAILED;
        } else {
            memset(newBackend->backendData, 0, sizeof(elosInfluxDbBackend_t));
            newBackend->name = BACKEND_NAME;
            newBackend->start = elosInfluxDbBackendStart;
            newBackend->persist = elosInfluxDbBackendPersist;
            newBackend->findEvent = elosInfluxDbBackendFindEvents;
            newBackend->shutdown = elosInfluxDbBackendShutdown;
            *backend = newBackend;
            result = SAFU_RESULT_OK;
        }
    }

    return result;
}
