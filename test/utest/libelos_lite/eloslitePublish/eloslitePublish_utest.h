// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/libelos_lite.h>

#include "../../src/libelos_lite/private/message_utils.h"

typedef struct elosliteTestMessageBuffer {
    size_t len;
    size_t cap;
    char *buf;
} elosliteTestMessageBuffer_t;

typedef struct elosliteTestData {
    elosliteSession_t session;
    elosliteEvent_t fullEvent;
    elosliteTestMessageBuffer_t eventMessage;
} elosliteTestData_t;

void elosliteTestMessageBufferNew(elosliteTestMessageBuffer_t *testBuffer, size_t cap);
int elosliteTestMessageBufferPush(elosliteTestMessageBuffer_t *testBuffer, const void *add, size_t len);
void elosliteTestMessageBufferReset(elosliteTestMessageBuffer_t *testBuffer);
void elosliteTestMessageBufferFree(elosliteTestMessageBuffer_t *testBuffer);
size_t elosliteCheckSendHeader(elosliteTestMessageBuffer_t *buffer);

TEST_CASE_FUNC_PROTOTYPES(elosliteTestEloslitePublishSuccessAllPermutations)
