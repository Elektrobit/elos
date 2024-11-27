// SPDX-License-Identifier: MIT

#include "message_utils.h"

#include <stdbool.h>
#include <string.h>

uint16_t elosliteNumLen(int64_t num) {
    uint16_t result = 0;
    if (num == 0) result = 1;
    while (num) {
        num /= 10;
        result += 1;
    }
    return result;
}

uint16_t elosliteMsgLen(elosliteEvent_t *event) {
    uint16_t msgLen = 2;
    bool elementBefore = false;
    if (event->date.tv_sec != 0 || event->date.tv_nsec != 0) {
        elementBefore = true;
        msgLen += strlen("\"date\":[,]");
        msgLen += elosliteNumLen(event->date.tv_sec);
        msgLen += elosliteNumLen(event->date.tv_nsec);
    }
    if (event->source.appName != NULL || event->source.fileName != NULL || event->source.pid != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"source\":{}");
        bool sourceElementBefore = false;
        if (event->source.appName != NULL) {
            sourceElementBefore = true;
            msgLen += strlen("\"appName\":\"\"");
            msgLen += strlen(event->source.appName);
        }
        if (event->source.fileName != NULL) {
            msgLen += sourceElementBefore ? 1 : 0;
            sourceElementBefore = true;
            msgLen += strlen("\"fileName\":\"\"");
            msgLen += strlen(event->source.fileName);
        }
        if (event->source.pid != 0) {
            msgLen += sourceElementBefore ? 1 : 0;
            msgLen += strlen("\"pid\":");
            msgLen += elosliteNumLen(event->source.pid);
        }
    }
    if (event->severity != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"severity\":");
        msgLen += elosliteNumLen(event->severity);
    }
    if (event->hardwareid != NULL) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"hardwareid\":\"\"");
        msgLen += strlen(event->hardwareid);
    }
    if (event->classification != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"classification\":");
        msgLen += elosliteNumLen(event->classification);
    }
    if (event->messageCode != 0) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"messageCode\":");
        msgLen += elosliteNumLen(event->messageCode);
    }
    if (event->payload != NULL) {
        msgLen += elementBefore ? 1 : 0;
        elementBefore = true;
        msgLen += strlen("\"payload\":\"\"");
        msgLen += strlen(event->payload);
    }
    return msgLen;
}
