// SPDX-License-Identifier: MIT

#include <elos/event/event.h>
#include <elos/libelos_lite.h>
#include <elos/lite/event.h>
#include <string.h>

#include "eloslitePublish_utest.h"

int elosliteTestEloslitePublishSuccessAllPermutationsSetup(UNUSED void **state) {
    elosliteTestData_t *data = malloc(sizeof(elosliteTestData_t));

    data->session.fd = 33;
    data->session.connected = true;

    data->fullEvent = (elosliteEvent_t){
        .date =
            {
                .tv_sec = 5000,
                .tv_nsec = 70,
            },
        .source =
            {
                .appName = "elosliteTestEloslitePublishSuccessAllPermutations",
                .fileName = "test/utest/libelos_lite/elosLitePublish/unit_test",
                .pid = 42,
            },
        .severity = ELOS_SEVERITY_INFO,
        .hardwareid = "380fde9d11494b9d6f58de2d2d99e261eb0eb6d9",
        .classification = ELOS_CLASSIFICATION_SECURITY,
        .messageCode = ELOS_MSG_CODE_INFO_LOG,
        .payload = "A test message payload...",
    };

    elosliteTestMessageBufferNew(&data->eventMessage, 50);

    *state = data;
    return 0;
}

int elosliteTestEloslitePublishSuccessAllPermutationsTeardown(void **state) {
    elosliteTestData_t *data = *state;
    elosliteTestMessageBufferFree(&data->eventMessage);
    free(*state);
    return 0;
}

static void _prepareSendMock(elosliteTestData_t *data) {
    elosliteSession_t *session = &data->session;
    elosliteTestMessageBuffer_t *buffer = &data->eventMessage;
    expect_value_count(__wrap_send, sockfd, session->fd, -1);
    expect_value_count(__wrap_send, flags, MSG_NOSIGNAL, -1);
    will_return_always(__wrap_send, buffer);
}

static void _testPublishSingleEvent(elosliteTestData_t *data, elosliteEvent_t *event) {
    elosliteSession_t *session = &data->session;
    elosliteTestMessageBuffer_t *buffer = &data->eventMessage;

    char bufferRep[] = "{\"error\":none}";
    size_t resLen = strlen(bufferRep) + 1;
    struct elosliteMessageHead replyHead = {
        .version = ELOS_PROTOCOL_VERSION,
        .message = ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH,
        .length = resLen,
    };

    will_return(__wrap_recv, sizeof(struct elosliteMessageHead));
    will_set_parameter(__wrap_recv, buf, &replyHead);

    will_return(__wrap_recv, resLen);
    will_set_parameter(__wrap_recv, buf, bufferRep);

    bool res = eloslitePublish(session, event);

    assert_int_equal(res, true);
    elosEvent_t resultEvent = {0};
    size_t sendLen = elosliteCheckSendHeader(buffer);
    size_t msgLen = strlen(&(buffer->buf)[sizeof(struct elosliteMessageHead)]);
    assert_int_equal(sendLen, msgLen);
    safuResultE_t desResult = elosEventDeserialize(&resultEvent, &(buffer->buf)[sizeof(struct elosliteMessageHead)]);
    assert_int_equal(desResult, SAFU_RESULT_OK);

    elosEventDeleteMembers(&resultEvent);
    elosliteTestMessageBufferReset(&data->eventMessage);
}

#define TV_SEC_MASK         1
#define TV_NSEC_MASK        (1 << 1)
#define SOURCE_MASK         (1 << 2)
#define APPNAME_MASK        (1 << 3)
#define FILENAME_MASK       (1 << 4)
#define PID_MASK            (1 << 5)
#define SEVERITY_MASK       (1 << 6)
#define HARDWAREID_MASK     (1 << 7)
#define CLASSIFICATION_MASK (1 << 8)
#define MESSAGECODE_MASK    (1 << 9)
#define PAYLOAD_MASK        (1 << 10)

void elosliteTestEloslitePublishSuccessAllPermutations(void **state) {
    elosliteTestData_t *data = *state;
    elosliteEvent_t *fullEvent = &data->fullEvent;
    elosliteSession_t *session = &data->session;
    TEST("elosliteTestEloslitePublishSuccessAllPermutations");
    SHOULD("%s", "successfully publish events with all kinds of permutations of fields");

    _prepareSendMock(data);
    expect_value_count(__wrap_recv, sockfd, session->fd, -1);
    expect_value_count(__wrap_recv, flags, 0, -1);

    for (int permutation = 0; permutation < (1 << 11); permutation++) {
        elosliteEvent_t event = (elosliteEvent_t){
            .date =
                {
                    .tv_sec = (permutation & TV_SEC_MASK) ? fullEvent->date.tv_sec : 0,
                    .tv_nsec = (permutation & TV_NSEC_MASK) ? fullEvent->date.tv_nsec : 0,
                },
            .source =
                {
                    .appName = (permutation & APPNAME_MASK) ? fullEvent->source.appName : NULL,
                    .fileName = (permutation & FILENAME_MASK) ? fullEvent->source.fileName : NULL,
                    .pid = (permutation & PID_MASK) ? fullEvent->source.pid : 0,
                },
            .severity = (permutation & SEVERITY_MASK) ? fullEvent->severity : 0,
            .hardwareid = (permutation & HARDWAREID_MASK) ? fullEvent->hardwareid : NULL,
            .classification = (permutation & CLASSIFICATION_MASK) ? fullEvent->classification : 0,
            .messageCode = (permutation & MESSAGECODE_MASK) ? fullEvent->messageCode : 0,
            .payload = (permutation & PAYLOAD_MASK) ? fullEvent->payload : NULL,
        };
        _testPublishSingleEvent(data, &event);
    }
}
