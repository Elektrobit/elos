// SPDX-License-Identifier: MIT

#include "kmsg_mapper.h"

#include <inttypes.h>
#include <limits.h>
#include <safu/common.h>
#include <safu/log.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

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

static safuResultE_t _getTimeOfBoot(struct timespec *bootTime) {
    safuResultE_t result = SAFU_RESULT_OK;
    struct timespec upTime;
    FILE *procUptime = fopen("/proc/uptime", "r");
    if (procUptime == NULL) {
        safuLogErr("opening /proc/uptime failed!");
        result = SAFU_RESULT_FAILED;
    }
    char buffer[50];
    if (result == SAFU_RESULT_OK) {
        if (fgets(buffer, ARRAY_SIZE(buffer), procUptime) == NULL) {
            safuLogErr("reading from /proc/uptime failed!");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (procUptime != NULL) {
        fclose(procUptime);
    }
    buffer[49] = 0;
    char *decimalPoints = NULL;
    if (result == SAFU_RESULT_OK) {
        upTime.tv_sec = strtoul(buffer, &decimalPoints, 10);
        if (buffer == decimalPoints || upTime.tv_sec == INT64_MAX || decimalPoints[0] != '.') {
            safuLogErr("parsing seconds since boot failed!");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        decimalPoints += 1;
        char *end = NULL;
        upTime.tv_nsec = strtoul(decimalPoints, &end, 10);
        if (decimalPoints == end || upTime.tv_nsec == INT64_MAX) {
            safuLogErr("parsing decimal points of seconds since boot failed!");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        upTime.tv_nsec *= 10000000;
        bootTime->tv_sec = time(NULL) - upTime.tv_sec - 1;
        bootTime->tv_nsec = 1000000000 - upTime.tv_nsec;
    }

    return result;
}

struct kmsgMsg {
    char *fullMsg;
    int prefix;
    uint64_t sequenceNumber;
    struct timespec timeStamp;
    char flag;
    char *msg;
};

static struct timespec _microsecsToTimespec(uint64_t microSec) {
    struct timespec timeStamp;
    timeStamp.tv_sec = microSec / 1000000;
    timeStamp.tv_nsec = (microSec % 1000000) * 1000;
    return timeStamp;
}

static safuResultE_t _parseKmsg(char *fullMessage, struct kmsgMsg *msg) {
    safuResultE_t result = SAFU_RESULT_OK;
    long int prefix;
    uint64_t sequenceNumber = 0;
    uint64_t timeStamp = 0;
    char flag = '\0';

    char *valueStart = fullMessage;
    char *valueEnd = NULL;
    prefix = strtol(valueStart, &valueEnd, 10);
    if (valueStart == valueEnd || prefix == LONG_MAX || prefix == LONG_MIN || valueEnd[0] != ',') {
        safuLogErr("parsing kmsg prefix failed");
        result = SAFU_RESULT_FAILED;
    }
    if (result == SAFU_RESULT_OK) {
        valueStart = valueEnd + 1;
        valueEnd = NULL;
        sequenceNumber = strtoul(valueStart, &valueEnd, 10);
        if (valueStart == valueEnd || sequenceNumber == UINT64_MAX || valueEnd[0] != ',') {
            safuLogErr("parsing kmsg sequence number failed");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        valueStart = valueEnd + 1;
        valueEnd = NULL;
        timeStamp = strtoul(valueStart, &valueEnd, 10);
        if (valueStart == valueEnd || timeStamp == UINT64_MAX || valueEnd[0] != ',') {
            safuLogErr("parsing kmsg timestamp failed");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        valueStart = valueEnd + 1;
        flag = valueStart[0];
        if (flag == 0) {
            safuLogErr("parsing kmsg no flag field");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        valueStart += 1;
        if (valueStart[0] != ';' || valueStart[1] == 0) {
            safuLogErr("kmsg text field is empty/dosnt exist");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (result == SAFU_RESULT_OK) {
        msg->fullMsg = fullMessage;
        msg->prefix = prefix;
        msg->sequenceNumber = sequenceNumber;
        msg->timeStamp = _microsecsToTimespec(timeStamp);
        msg->flag = flag;
        msg->msg = &valueStart[1];
    }
    return result;
}

static struct timespec _monotonicToGlobalClock(const elosKmsgMapper_t *mapper, struct timespec mono) {
    mono.tv_nsec += mapper->bootTime.tv_nsec;
    mono.tv_sec += mapper->bootTime.tv_sec;
    return mono;
}

safuResultE_t elosKmsgMapperInit(elosKmsgMapper_t *mapper) {
    return _getTimeOfBoot(&mapper->bootTime);
}

safuResultE_t elosKmsgMapperDoMapping(const elosKmsgMapper_t *mapper, elosEvent_t *event, char *kmsgLine) {
    struct kmsgMsg kmsg = {0};
    safuResultE_t res = _parseKmsg(kmsgLine, &kmsg);
    if (res == SAFU_RESULT_OK) {
        event->messageCode = ELOS_MSG_CODE_KMSG;
        event->payload = kmsg.fullMsg;
        event->severity = elosSeverityMapping[kmsg.prefix & 0x07];
        event->classification |= elosClassificationMapping[kmsg.prefix >> 3];
        event->date = _monotonicToGlobalClock(mapper, kmsg.timeStamp);
    }
    return res;
}
