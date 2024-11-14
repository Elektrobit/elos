// SPDX-License-Identifier: MIT

#include <string.h>

#include "eloslitePublish_utest.h"

void elosliteTestMessageBufferNew(elosliteTestMessageBuffer_t *testBuffer, size_t cap) {
    testBuffer->buf = malloc(cap);
    testBuffer->cap = cap;
    testBuffer->len = 0;
}

int elosliteTestMessageBufferPush(elosliteTestMessageBuffer_t *testBuffer, const void *add, size_t len) {
    if (testBuffer->cap - testBuffer->len <= len) {
        testBuffer->cap *= 2;
        testBuffer->buf = realloc(testBuffer->buf, testBuffer->cap);
    }
    memcpy(testBuffer->buf + testBuffer->len, add, len);
    testBuffer->len += len;
    testBuffer->buf[testBuffer->len] = '\0';
    return len;
}

void elosliteTestMessageBufferReset(elosliteTestMessageBuffer_t *testBuffer) {
    testBuffer->len = 0;
}

void elosliteTestMessageBufferFree(elosliteTestMessageBuffer_t *testBuffer) {
    free(testBuffer->buf);
}

size_t elosliteCheckSendHeader(elosliteTestMessageBuffer_t *buffer) {
    struct elosliteMessageHead *msgHead = (struct elosliteMessageHead *)buffer->buf;
    assert_int_equal(msgHead->version, ELOS_PROTOCOL_VERSION);
    assert_int_equal(msgHead->message, ELOS_MESSAGE_EVENT_PUBLISH);
    return msgHead->length;
}

ssize_t __wrap_send(int sockfd, const void *buf, size_t len, int flags) {
    check_expected(sockfd);
    check_expected(flags);
    elosliteTestMessageBuffer_t *buffer = mock_ptr_type(elosliteTestMessageBuffer_t *);
    elosliteTestMessageBufferPush(buffer, buf, len);
    return len;
}

ssize_t __wrap_recv(int sockfd, const void *buf, size_t len, int flags) {
    check_expected(sockfd);
    check_expected(flags);
    size_t size = mock_type(ssize_t);
    assert_true(len >= size);
    memcpy((void *)buf, mock_ptr_type(void *), size);
    return size;
}
