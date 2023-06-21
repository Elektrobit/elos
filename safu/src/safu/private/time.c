// SPDX-License-Identifier: MIT
#include "safu/time.h"

#include <stdlib.h>

#include "safu/log.h"

safuResultE_t safuTimeGetLocalTime(struct tm *localTime) {
    time_t currentTimestamp = 0;
    int result = SAFU_RESULT_OK;

    if (time(&currentTimestamp) == -1) {
        safuLogErrErrno("time() failed");
        result = SAFU_RESULT_FAILED;
    } else {
        if (localtime_r(&currentTimestamp, localTime) == NULL) {
            safuLogErrErrno("localtime_r() failed");
            result = SAFU_RESULT_FAILED;
        }
    }
    return result;
}

#define DATE_MAX_LEN 100
char *safuGetCurrentDateString(char *dateFormat) {
    char *date = NULL;
    if (dateFormat == NULL) {
        safuLogErr("Invalid Date format received");
    } else {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        if (t == NULL) {
            safuLogErr("Failed to allocate time struct");
        } else {
            date = safuAllocMem(NULL, sizeof(char) * DATE_MAX_LEN);
            if (date == NULL) {
                safuLogErr("Failed to allocate date string");
            }
            size_t len = strftime(date, DATE_MAX_LEN - 1, dateFormat, t);
            if (len == 0) {
                safuLogErrF("could not correctly produce date string for formt %s", dateFormat);
                free(date);
                date = NULL;
            }
        }
    }
    return date;
}
