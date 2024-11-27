// SPDX-License-Identifier: MIT

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/resource.h>

#include "elos/libelos_lite.h"
#include "elos/lite/event.h"
#include "message_utils.h"

static int _sendFlush(elosliteSession_t *session, elosliteBuffer_t *buffer);
static int _sendPartF(elosliteSession_t *session, elosliteBuffer_t *buffer, const char *fmt, ...);
static int _sendPartStr(elosliteSession_t *session, elosliteBuffer_t *buffer, const char *str);
static int _sendMsgParts(elosliteSession_t *session, elosliteEvent_t *event);
static bool _msgHandleResponse(elosliteSession_t *session);

bool eloslitePublish(elosliteSession_t *session, elosliteEvent_t *event) {
    if (session == NULL || event == NULL || session->connected == false) {
        return false;
    }
    struct elosliteMessageHead msgHead = {
        .version = ELOS_PROTOCOL_VERSION,
        .message = ELOS_MESSAGE_EVENT_PUBLISH,
        .length = elosliteMsgLen(event),
    };
    int sendNum = send(session->fd, (void *)&msgHead, sizeof(msgHead), MSG_NOSIGNAL);
    if (sendNum < 0) {
        elosliteDisconnect(session);
        return false;
    }
    sendNum = _sendMsgParts(session, event);
    if (sendNum != msgHead.length) {
        elosliteDisconnect(session);
        return false;
    }
    if (!_msgHandleResponse(session)) {
        elosliteDisconnect(session);
        return false;
    }
    return true;
}

static int _sendFlush(elosliteSession_t *session, elosliteBuffer_t *buffer) {
    int sendNum = send(session->fd, (void *)buffer->buffer, buffer->pos, MSG_NOSIGNAL);
    buffer->pos = 0;
    buffer->buffer[0] = 0;
    return sendNum;
}

static int _sendPartF(elosliteSession_t *session, elosliteBuffer_t *buffer, const char *fmt, ...) {
    int sendNum = 0;
    int sizeLeft = buffer->size - buffer->pos;
    va_list args;
    va_start(args, fmt);
    int off = vsnprintf(&buffer->buffer[buffer->pos], sizeLeft, fmt, args);
    va_end(args);
    if (off < 0) {
        return -1;
    } else if (off >= sizeLeft) {
        buffer->buffer[buffer->pos] = 0;
        sendNum = _sendFlush(session, buffer);
        va_list rargs;
        va_start(rargs, fmt);
        off = vsnprintf(buffer->buffer, buffer->size, fmt, rargs);
        va_end(rargs);
        if (off < 0 || (size_t)off > buffer->size) {
            return -1;
        } else {
            buffer->pos += off;
        }
    } else {
        buffer->pos += off;
    }
    return sendNum;
}

static int _sendPartStr(elosliteSession_t *session, elosliteBuffer_t *buffer, const char *str) {
    int sendNum = 0;
    size_t strLen = strlen(str);
    if (strLen < buffer->size) {
        sendNum = _sendPartF(session, buffer, str);
    } else {
        sendNum = _sendFlush(session, buffer);
        if (sendNum >= 0) {
            int sendStrLen = send(session->fd, (void *)str, strLen, MSG_NOSIGNAL);
            if (sendStrLen < 0) {
                return sendStrLen;
            }
            sendNum += sendStrLen;
        }
    }
    return sendNum;
}

static int _sendMsgParts(elosliteSession_t *session, elosliteEvent_t *event) {
    elosliteBuffer_t buffer = {0};
    buffer.pos = 0;
#ifndef ELOSLITE_SEND_BUFFER_SIZE
    buffer.size = getauxval(AT_PAGESZ);
#else
    buffer.size = ELOSLITE_SEND_BUFFER_SIZE;
#endif
    char *partBuf = alloca(buffer.size);
    buffer.buffer = partBuf;
    size_t sendNum = 0;
    sendNum += _sendPartF(session, &buffer, "{");
    bool elementBefore = false;
    if (event->date.tv_sec != 0 || event->date.tv_nsec != 0) {
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"date\":[%ld,%ld]", event->date.tv_sec, event->date.tv_nsec);
    }
    if (event->source.appName != NULL || event->source.fileName != NULL || event->source.pid != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"source\":{");
        bool sourceElementBefore = false;
        if (event->source.appName != NULL) {
            sourceElementBefore = true;
            sendNum += _sendPartF(session, &buffer, "\"appName\":\"");
            sendNum += _sendPartStr(session, &buffer, event->source.appName);
            sendNum += _sendPartF(session, &buffer, "\"");
        }
        if (event->source.fileName != NULL) {
            if (sourceElementBefore) {
                sendNum += _sendPartF(session, &buffer, ",");
            }
            sourceElementBefore = true;
            sendNum += _sendPartF(session, &buffer, "\"fileName\":\"");
            sendNum += _sendPartStr(session, &buffer, event->source.fileName);
            sendNum += _sendPartF(session, &buffer, "\"");
        }
        if (event->source.pid != 0) {
            if (sourceElementBefore) {
                sendNum += _sendPartF(session, &buffer, ",");
            }
            sendNum += _sendPartF(session, &buffer, "\"pid\":%d", event->source.pid);
        }
        sendNum += _sendPartF(session, &buffer, "}", event->source.pid);
    }
    if (event->severity != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"severity\":%d", event->severity);
    }
    if (event->hardwareid != NULL) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"hardwareid\":\"");
        sendNum += _sendPartStr(session, &buffer, event->hardwareid);
        sendNum += _sendPartF(session, &buffer, "\"");
    }
    if (event->classification != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"classification\":%ld", event->classification);
    }
    if (event->messageCode != 0) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"messageCode\":%d", event->messageCode);
    }
    if (event->payload != NULL) {
        if (elementBefore) {
            sendNum += _sendPartF(session, &buffer, ",");
        }
        elementBefore = true;
        sendNum += _sendPartF(session, &buffer, "\"payload\":\"");
        sendNum += _sendPartStr(session, &buffer, event->payload);
        sendNum += _sendPartF(session, &buffer, "\"");
    }
    sendNum += _sendPartF(session, &buffer, "}");
    sendNum += _sendFlush(session, &buffer);
    return sendNum;
}

static bool _msgHandleResponse(elosliteSession_t *session) {
    struct elosliteMessageHead msgHead = {0};
    recv(session->fd, &msgHead, sizeof(msgHead), 0);
    if ((msgHead.message != ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH) || (msgHead.version != ELOS_PROTOCOL_VERSION)) {
        return false;
    }
    char buf[20];
    uint16_t lengthRecv = 0;
    while (lengthRecv < msgHead.length) {
        int res = recv(session->fd, buf, 20, 0);
        if (res < 0) {
            return false;
        }
        lengthRecv += res;
    }
    return true;
}
