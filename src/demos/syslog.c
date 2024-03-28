// SPDX-License-Identifier: MIT

#include <errno.h>
#include <safu/common.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/libelos/libelos.h"

#ifndef _LOG_PATH
#define _LOG_PATH "/dev/log"
#endif

bool elosSendSyslogMessage(const char *message);

void _printUsage(FILE *stream, const char *const elosProgramName) {
    fprintf(stream,
            "Usage: %s [OPTION]...\n"
            "  -m   syslog message\n"
            "  -H   set host ip address\n"
            "  -P   set host port\n"
            "  -h   print this help\n",
            elosProgramName);
}

int main(int argc, char *argv[]) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosSession_t *session;
    char *host = "127.0.0.1";
    int port = 54321;
    int count = 0;
    char *header = "<38>Jan  1 01:41:57 sshd[240]";
    char *payload = "test syslog daemon";

    while (result == SAFU_RESULT_OK) {
        count = getopt(argc, argv, "m:H:P:h");
        if (count == -1) {
            break;
        }
        switch (count) {
            case 'm':
                payload = optarg;
                break;
            case 'H':
                host = optarg;
                break;
            case 'P':
                port = strtol(optarg, NULL, 10);
                break;
            case 'h':
                _printUsage(stdout, argv[0]);
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "ERROR: Unrecognized option: '-%c'\n", optopt);
                return EXIT_FAILURE;
        }
    }
    char *message = malloc(strlen(header) + strlen(payload) + 1);

    if (message == NULL) {
        fprintf(stderr, "failed to allocate message\n");
        return EXIT_FAILURE;
    }

    char *eventRuleBase = ".event.payload '%s' STRCMP";
    char *eventRule = malloc(strlen(eventRuleBase) + strlen(payload));

    if (eventRule == NULL) {
        free(message);
        fprintf(stderr, "failed to allocate eventRule string\n");
        return EXIT_FAILURE;
    }

    const char *elosEvent[] = {eventRule};
    strcpy(message, header);
    strcat(message, payload);
    sprintf(eventRule, eventRuleBase, payload);

    result = elosConnectTcpip(host, port, &session);
    if (result != SAFU_RESULT_OK) {
        free(message);
        free(eventRule);
        fprintf(stderr, "failed to connect to elos\n");
        return EXIT_FAILURE;
    }

    // create an event sink for SYSLOG-event (subscribe to SYSLOG events)
    elosEventQueueId_t eventQueue = ELOS_ID_INVALID;
    result = elosEventSubscribe(session, elosEvent, ARRAY_SIZE(elosEvent), &eventQueue);

    if (result != SAFU_RESULT_OK) {
        fprintf(stderr, "failed to subscribe for syslog event\n");
        free(message);
        free(eventRule);
        elosDisconnect(session);
        return EXIT_FAILURE;
    }

    // publish a syslog message via /dev/log
    if (!elosSendSyslogMessage(message)) {
        elosDisconnect(session);
        free(eventRule);
        free(message);
        return EXIT_FAILURE;
    }
    free(message);

    // fetch syslog event
    elosEvent_t *event = NULL;
    elosEventVector_t *eventVector = NULL;
    printf("fetch events for %s(%u) ...\n", eventRule, eventQueue);
    free(eventRule);

    int readRetries = 10;
    while (readRetries > 0) {
        result = elosEventQueueRead(session, eventQueue, &eventVector);
        if (result != SAFU_RESULT_OK) {
            fprintf(stderr, "failed to read EventQueue for syslog event\n");
            elosDisconnect(session);
            return EXIT_FAILURE;
        }

        if (eventVector != NULL && eventVector->elementCount > 0) {
            event = safuVecGetLast(eventVector);
            if (event != NULL) {
                char *jsonString = NULL;
                result = elosEventSerialize(&jsonString, event);
                if (result != SAFU_RESULT_FAILED) {
                    printf("[receive message] %s\n", jsonString);
                    free(jsonString);
                }
            }
            elosEventVectorDelete(eventVector);
            eventVector = NULL;
            break;
        } else {
            elosEventVectorDelete(eventVector);
            eventVector = NULL;
            readRetries--;
            printf("empty event queue, retries left %d\n", readRetries);
            usleep(200000);
        }
    }

    result = elosDisconnect(session);
    if (result != SAFU_RESULT_OK) {
        fprintf(stderr, "failed to disconnect");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool elosSendSyslogMessage(const char *message) {
    int retval = 0;

    char *syslogSocketPath = getenv("ELOS_SYSLOG_PATH");
    if (syslogSocketPath == NULL) {
        syslogSocketPath = _LOG_PATH;
    }

    printf("Using syslogSocketPath: %s\n", syslogSocketPath);

    retval = access(syslogSocketPath, F_OK);
    if (retval < 0) {
        fprintf(stderr, "failed to access '%s' (%s)\n", syslogSocketPath, strerror(errno));
        return false;
    }

    int syslogSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (syslogSocket == -1) {
        perror("failed to create socket");
        return false;
    }

    struct sockaddr_un addr = {.sun_family = AF_UNIX};
    strncpy(addr.sun_path, syslogSocketPath, sizeof(addr.sun_path) - 1);
    retval = connect(syslogSocket, (const struct sockaddr *)&addr, sizeof(addr));
    if (retval == -1) {
        fprintf(stderr, "failed to connect to syslog socket %s:%s\n", addr.sun_path, strerror(errno));
        return false;
    }

    size_t const bytesToSend = strnlen(message, 200);
    ssize_t bytes;
    printf("[send message] %s\n", message);
    bytes = send(syslogSocket, message, bytesToSend, 0);
    if (bytes < 0) {
        fprintf(stderr, "send failed (%s)\n", strerror(errno));
        return false;
    } else if (bytes != (ssize_t)bytesToSend) {
        char const errStr[] = "send failed; message had %lu bytes, but only %lu where sent (%s)\n";
        fprintf(stderr, errStr, bytesToSend, bytes, strerror(errno));
    } else {
        printf("[send message] %lu bytes written\n", bytes);
    }

    // make sure event is delivered
    usleep(1000000);

    retval = close(syslogSocket);
    if (retval < 0) {
        perror("close failed");
        return false;
    }

    return true;
}
