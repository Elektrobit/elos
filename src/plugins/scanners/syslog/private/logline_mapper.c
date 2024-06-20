// SPDX-License-Identifier: MIT
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <bits/types/struct_timespec.h>
#include <safu/common.h>
#include <samconf/samconf.h>

#include "elos/event/event_message_codes.h"
#include "elos/eventfilter/eventfilter.h"
#include "safu/common.h"
#include "safu/vector.h"
#endif

#include <bits/types/locale_t.h>
#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "elos/event/event.h"
#include "elos/event/event_source.h"
#include "elos/rpnfilter/builder.h"
#include "elos/scanner_legacy/scanner.h"
#include "logline_mapper.h"
#include "safu/log.h"
#include "safu/time.h"

static const char *elosElosDateTemplate = "%h %e %T ";

// clang-format off
static const int elosSeverityMapping[8] = {
    [0] = ELOS_SEVERITY_FATAL,
    [1] = ELOS_SEVERITY_ERROR,
    [2] = ELOS_SEVERITY_ERROR,
    [3] = ELOS_SEVERITY_WARN,
    [4] = ELOS_SEVERITY_WARN,
    [5] = ELOS_SEVERITY_INFO,
    [6] = ELOS_SEVERITY_INFO,
    [7] = ELOS_SEVERITY_DEBUG,
};
// clang-format on

// clang-format off
static const uint64_t elosClassificationMapping[24] = {
    [0] = ELOS_CLASSIFICATION_KERNEL,
    [1] = ELOS_CLASSIFICATION_UNDEFINED,
    [2] = ELOS_CLASSIFICATION_NETWORK,
    [3] = ELOS_CLASSIFICATION_PROCESS,
    [4] = ELOS_CLASSIFICATION_SECURITY,
    [5] = ELOS_CLASSIFICATION_UNDEFINED,
    [6] = ELOS_CLASSIFICATION_UNDEFINED,
    [7] = ELOS_CLASSIFICATION_NETWORK,
    [8] = ELOS_CLASSIFICATION_NETWORK | ELOS_CLASSIFICATION_IPC,
    [9] = ELOS_CLASSIFICATION_UNDEFINED,
    [10] = ELOS_CLASSIFICATION_SECURITY,
    [11] = ELOS_CLASSIFICATION_NETWORK,
    [12] = ELOS_CLASSIFICATION_NETWORK,
    [13] = ELOS_CLASSIFICATION_SECURITY,
    [14] = ELOS_CLASSIFICATION_UNDEFINED,
    [15] = ELOS_CLASSIFICATION_UNDEFINED,
    [16] = ELOS_CLASSIFICATION_USER_0,
    [17] = ELOS_CLASSIFICATION_USER_1,
    [18] = ELOS_CLASSIFICATION_USER_2,
    [19] = ELOS_CLASSIFICATION_USER_3,
    [20] = ELOS_CLASSIFICATION_USER_4,
    [21] = ELOS_CLASSIFICATION_USER_5,
    [22] = ELOS_CLASSIFICATION_USER_6,
    [23] = ELOS_CLASSIFICATION_USER_7,
};
// clang-format on

typedef struct elosMessageCodeFilterMapping {
    elosEventMessageCodeE_t messageCode;
    elosEventFilter_t filter;
} elosMessageCodeFilterMapping_t;

static const char *_parsePRI(const char *logline, int *pri) {
    char *nextColumn = NULL;
    if (logline[0] == '<' && strchr(logline, '>') != NULL) {
        *pri = strtol(&logline[1], &nextColumn, 10);
        nextColumn++;
    }
    return nextColumn;
}

static void _mapPRI(elosEvent_t *event, int pri) {
    event->severity = elosSeverityMapping[pri & 0x07];
    event->classification |= elosClassificationMapping[pri >> 3];
}

static const char *_parseTime(const char *date, struct timespec *eventTime) {
    struct tm tmp = {0};
    time_t timestamp = 0;
    const char *remaining = NULL;

    eventTime->tv_sec = 0;
    eventTime->tv_nsec = 0;

    if (safuTimeGetLocalTime(&tmp) == SAFU_RESULT_OK) {
        locale_t useLocale = newlocale(LC_TIME, "C", 0);
        if (useLocale == (locale_t)0) {
            safuLogErrErrno("newlocale failed, use current time");
        } else {
            remaining = strptime_l(date, elosElosDateTemplate, &tmp, useLocale);
            if (remaining == NULL) {
                safuLogErrErrno("strptime_l failed, use current time");
            } else {
                timestamp = mktime(&tmp);
                if (timestamp == -1) {
                    timestamp = 0;
                    safuLogErrErrno("mktime failed, use current time");
                }
            }
        }
    }

    eventTime->tv_sec = timestamp;
    return remaining;
}

static const char *_parseSource(const char *identifier, elosEventSource_t *source) {
    size_t appNameLength = 0;
    const char *nextColumn = identifier;
    const char *pid = strchr(identifier, '[');
    if (pid != NULL) {
        source->pid = strtol(&pid[1], (char **)&nextColumn, 10);
        appNameLength = pid - identifier;
    } else {
        const char *payloadDelimiter = strchr(identifier, ':');
        if (payloadDelimiter == NULL) {
            safuLogErr("failed to parse source identifier");
            safuLogDebugF("invalid syslog format at '%s'", identifier);
        } else {
            appNameLength = payloadDelimiter - identifier;
            nextColumn = &identifier[appNameLength];
        }
    }
    source->appName = strndup(identifier, appNameLength);
    return nextColumn;
}

static safuResultE_t _mapMessageCode(elosEvent_t *event, const safuVec_t *filterList) {
    elosRpnFilterResultE_t result = RPNFILTER_RESULT_OK;
    elosMessageCodeFilterMapping_t *mapping = NULL;
    size_t filterCount = safuVecElements(filterList);

    for (size_t i = 0; i < filterCount; i++) {
        mapping = safuVecGet(filterList, i);
        if (mapping == NULL) {
            safuLogErrF("Filter list at %ld is NULL", i);
            continue;
        }

        result = elosEventFilterExecute(&mapping->filter, NULL, event);

        if (result == RPNFILTER_RESULT_MATCH) {
            event->messageCode = mapping->messageCode;
            break;
        } else if (result == RPNFILTER_RESULT_ERROR) {
            safuLogErr("Failed to execute filter");
        }
    }

    return result == RPNFILTER_RESULT_MATCH ? SAFU_RESULT_OK : SAFU_RESULT_FAILED;
}

static inline bool _isNumericMessageCode(const char *key) {
    char *digitCheck = NULL;
    strtol(key, &digitCheck, 10);
    return *digitCheck == '\0';
}

static ssize_t _getFilterRuleCount(const samconfConfig_t *config) {
    size_t ruleCount = 0;
    size_t childCount = 0;

    if (config == NULL) {
        return -1;
    }
    childCount = config->childCount;

    for (size_t childItr = 0; childItr < childCount; childItr++) {
        if (config->children[childItr]->type == SAMCONF_CONFIG_VALUE_STRING &&
            _isNumericMessageCode(config->children[childItr]->key)) {
            ++ruleCount;
        }
    }

    return ruleCount;
}

static safuResultE_t _getFilterRule(const samconfConfig_t *config, size_t *ruleIdx, size_t filterCount,
                                    const char **rules, long *keys) {
    size_t childCount = 0;
    safuResultE_t retval = SAFU_RESULT_OK;

    if (config != NULL) {
        childCount = config->childCount;
        for (size_t childItr = 0; childItr < childCount; childItr++) {
            if (config->children[childItr]->type == SAMCONF_CONFIG_VALUE_STRING) {
                if (_isNumericMessageCode(config->children[childItr]->key)) {
                    if (*ruleIdx < filterCount) {
                        rules[*ruleIdx] = config->children[childItr]->value.string;
                        keys[*ruleIdx] = strtol(config->children[childItr]->key, NULL, 10);
                        ++(*ruleIdx);
                    }
                } else {
                    safuLogErrF("invalid config: MessageCode \"%s\" has wrong formatting!",
                                config->children[childItr]->key);
                }
            } else {
                safuLogErrF("invalid config: filter under MessageCode \"%s\" is not a valid filter string!",
                            config->children[childItr]->key);
            }
        }
    } else {
        safuLogErr("Invalid or null config to collect filter rules");
        retval = SAFU_RESULT_FAILED;
    }

    return retval;
}

safuResultE_t _createFilterList(ssize_t *filterRuleCount, const char **filterRules, long *filterKeys,
                                safuVec_t *filterList) {
    safuResultE_t result = SAFU_RESULT_OK;
    int retValue = safuVecCreate(filterList, *filterRuleCount, sizeof(elosMessageCodeFilterMapping_t));
    if (retValue == 0) {
        for (ssize_t i = 0; i < *filterRuleCount; i++) {
            elosMessageCodeFilterMapping_t mapping = {.messageCode = filterKeys[i]};
            const elosEventFilterParam_t param = {.filterString = filterRules[i]};

            elosRpnFilterResultE_t filterResult = elosEventFilterCreate(&mapping.filter, &param);
            if (filterResult == RPNFILTER_RESULT_ERROR) {
                safuLogWarnF("Failed to create filter for rule '%s'", filterRules[i]);
            } else {
                retValue = safuVecPush(filterList, &mapping);
                if (retValue != 0) {
                    safuLogWarn("Failed to insert filter into filter list");
                    elosEventFilterDeleteMembers(&mapping.filter);
                }
            }
        }
    } else {
        safuLogErr("Failed to create vector of filters");
        result = SAFU_RESULT_FAILED;
    }
    return result;
}

safuResultE_t _loadFilterRules(ssize_t *filterRuleCount, const samconfConfig_t *config, const char ***filterRules,
                               long **filterKeys) {
    safuResultE_t result = SAFU_RESULT_OK;
    size_t ruleIdx = 0;

    *filterRuleCount = _getFilterRuleCount(config);
    if (*filterRuleCount >= 0) {
        *filterRules = safuAllocMem(NULL, *filterRuleCount * sizeof(const char *));
        if (!*filterRules) {
            result = SAFU_RESULT_FAILED;
        } else {
            *filterKeys = safuAllocMem(NULL, *filterRuleCount * sizeof(const long *));
            if (!*filterKeys)
                result = SAFU_RESULT_FAILED;
            else
                result = _getFilterRule(config, &ruleIdx, *filterRuleCount, *filterRules, *filterKeys);
        }
        if (result != SAFU_RESULT_OK) {
            safuLogErr("Failed to collect filter rules");
            result = SAFU_RESULT_FAILED;
        }
    } else {
        safuLogErr("No message code filters found in given application configurations");
        result = SAFU_RESULT_FAILED;
    }
    return result;
}

safuResultE_t elosLoglineMapperInit(elosLoglineMapper_t *mapper, const samconfConfig_t *config) {
    ssize_t filterRuleCount = 0;
    const samconfConfig_t *messageCodeConfig = NULL;
    const char **filterRules = NULL;
    long *filterKeys = NULL;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    safuResultE_t result = SAFU_RESULT_OK;

    if (config == NULL) {
        safuLogErr("Configuration or mapper passed is null");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        status = samconfConfigGet(config, "Config/MappingRules/MessageCodes", &messageCodeConfig);
        if (status != SAMCONF_CONFIG_OK) {
            safuLogErr("Given configuration does not have applications config nodes");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        result = _loadFilterRules(&filterRuleCount, messageCodeConfig, &filterRules, &filterKeys);
    }

    if (result == SAFU_RESULT_OK) {
        result = _createFilterList(&filterRuleCount, filterRules, filterKeys, &mapper->filterList);
    }

    free(filterRules);
    free(filterKeys);
    return result;
}

safuResultE_t elosLoglineMapperDeleteMembers(elosLoglineMapper_t *mapper) {
    elosMessageCodeFilterMapping_t *mapping;
    size_t filterCount = safuVecElements(&mapper->filterList);

    for (size_t i = 0; i < filterCount; i++) {
        mapping = (elosMessageCodeFilterMapping_t *)safuVecGet(&mapper->filterList, i);
        elosEventFilterDeleteMembers(&mapping->filter);
    }

    safuVecFree(&mapper->filterList);

    return SAFU_RESULT_OK;
}

// syslog  Format:
// PRI VERSION TIMESTAMP HOSTNAME APP-NAME PROCID MSGID STRUCTURED-DATA MSG
//
safuResultE_t elosLoglineMapperDoMapping(elosLoglineMapper_t *mapper, elosEvent_t *event, const char *logLine) {
    int pri = 0;

    const char *next = _parsePRI(logLine, &pri);
    if (next != NULL) {
        _mapPRI(event, pri);
        next = _parseTime(next, &event->date);
        if (next != NULL) {
            next = _parseSource(next, &event->source);
        }
        if (next != NULL) {
            event->payload = strdup(&next[1]);
        }
        if (_mapMessageCode(event, &mapper->filterList) != SAFU_RESULT_OK) {
            safuLogWarn("Failed to map message code");
        }
    } else {
        safuLogWarnF("invalid syslog format:%s", logLine);
        event->payload = strdup(logLine);
    }

    return 0;
}
