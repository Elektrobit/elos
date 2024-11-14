// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/libelos_lite.h>

#define ELOS_PROTOCOL_VERSION               0x01
#define ELOS_MESSAGE_EVENT_PUBLISH          0x02
#define ELOS_MESSAGE_RESPONSE_BIT           0x80
#define ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH (ELOS_MESSAGE_EVENT_PUBLISH | ELOS_MESSAGE_RESPONSE_BIT)

struct elosliteMessageHead {
    uint8_t version;
    uint8_t message;
    uint16_t length;
};

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
