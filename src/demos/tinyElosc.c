// SPDX-License-Identifier: MIT
#define _GNU_SOURCE
#include <arpa/inet.h>
#include <json-c/json.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "elos/common/message.h"
#include "safu/json.h"

#ifndef IP_ADDRESS
#define IP_ADDRESS "127.0.0.1"
#endif

#ifndef PORT
#define PORT 54323
#endif

#define MAX_INPUT_JSON_SIZE 4096

static ssize_t _sendMessage(int socket, char *buffer, size_t len) {
    elosMessage_t *m = (elosMessage_t *)buffer;
    ssize_t sendBytes;

    fprintf(stdout, "send %ld bytes to server on port %d -> v=0x%02x;m=0x%02x;l=%u;j=%s\n", len, PORT, m->version,
            m->message, m->length, m->json);

    sendBytes = send(socket, buffer, len, MSG_NOSIGNAL);
    if (sendBytes <= 0) {
        fprintf(stderr, "ERR: send failed!\n");
    }

    return sendBytes;
}

static ssize_t _receiveMessage(int socket, char *buffer, size_t len) {
    elosMessage_t *m = (elosMessage_t *)buffer;
    ssize_t recvBytes;

    recvBytes = recv(socket, buffer, len, 0);
    if (recvBytes <= 0) {
        fprintf(stderr, "ERR: recv failed!\n");
    } else {
        fprintf(stdout, "received %ld bytes from server -> v=0x%02x;m=0x%02x;l=%u;j=%s\n", recvBytes, m->version,
                m->message, m->length, m->json);
    }

    return recvBytes;
}

static int _getEventQueueIdFromJsonString(const char *sResponse) {
    struct json_object *jResponse = NULL;
    int result = -1;

    jResponse = json_tokener_parse(sResponse);
    if (jResponse == NULL) {
        fprintf(stderr, "ERR: parsing \"%s\" failed\n", sResponse);
    } else {
        struct json_object *jData;

        jData = json_object_object_get(jResponse, "error");
        if (jData != NULL) {
            char const *err;

            err = json_object_get_string(jData);
            if (err == NULL) {
                result = 0;
            }
        }

        if (result == 0) {
            jData = json_object_object_get(jResponse, "eventQueueIds");
            if (jData == NULL) {
                fprintf(stderr, "ERR: get eventQueueIds from \"%s\" failed\n", sResponse);
                result = -1;
            } else {
                struct json_object *jEventQueueId;

                jEventQueueId = json_object_array_get_idx(jData, 0);
                if (jEventQueueId == NULL) {
                    fprintf(stderr, "ERR: get element of eventQueueIds\n");
                    result = -1;
                } else {
                    result = json_object_get_int(jEventQueueId);
                    fprintf(stdout, "eventqueueid=%d\n", result);
                }
            }
        }

        json_object_put(jResponse);
    }

    return result;
}

static char *_createEventMessage(char *jsonString, uint16_t len, int flag) {
    char *buffer;

    buffer = calloc(1, sizeof(elosMessage_t) + len);
    if (buffer == NULL) {
        fprintf(stderr, "ERR: memory allocation failed\n");
    } else {
        elosMessage_t *message = (elosMessage_t *)buffer;

        message->version = 0;
        message->message = flag;
        message->length = len;

        strncpy(message->json, jsonString, len);
    }

    return buffer;
}

static int _sendElosEvent(int socket, char *jsonString, char *buffer, size_t buflen, int flags) {
    uint16_t const maxLen = buflen - sizeof(elosMessage_t) - 1;
    uint16_t const len = strnlen(jsonString, maxLen) + 1;
    char *message;
    int result = -1;

    message = _createEventMessage(jsonString, len, flags);
    if (message != NULL) {
        ssize_t const bytesToSend = sizeof(elosMessage_t) + len;
        ssize_t bytes;

        memset(buffer, '\0', buflen);
        bytes = _sendMessage(socket, message, bytesToSend);
        if (bytes != bytesToSend) {
            fprintf(stderr, "ERR: sending message failed\n");
        } else {
            bytes = _receiveMessage(socket, buffer, buflen);
            if (bytes < (ssize_t)sizeof(elosMessage_t)) {
                fprintf(stderr, "ERR: receiving message failed\n");
            } else {
                result = 0;
            }
        }

        free(message);
    }

    return result;
}

static int _getVersion(int socket, char *buffer, size_t buflen) {
    char *message = "\x00\x01\x00\x00";
    int result = -1;
    ssize_t const bytesToSend = sizeof(elosMessage_t);
    ssize_t bytes;

    fprintf(stdout, "fetching elos version...\n");

    bytes = _sendMessage(socket, message, 4);
    if (bytes != bytesToSend) {
        fprintf(stderr, "ERR: sending message failed\n");
    } else {
        bytes = _receiveMessage(socket, buffer, buflen);
        if (bytes < (ssize_t)sizeof(elosMessage_t)) {
            fprintf(stderr, "ERR: receiving message failed\n");
        } else {
            result = 0;
        }
    }

    fprintf(stdout, "\n");

    return result;
}

static int _eventSubscribe(int socket, char *filterRule, char *buffer, size_t buflen) {
    char *sRequest = NULL;
    int result;

    fprintf(stdout, "subscribing to event...\n");

    result = asprintf(&sRequest, "{\"filter\":[\"%s\"]}", filterRule);
    if (result < 0) {
        fprintf(stderr, "ERR: request string creation failed\n");
    } else {
        result = _sendElosEvent(socket, sRequest, buffer, buflen, ELOS_MESSAGE_EVENT_SUBSCRIBE);
        if (result < 0) {
            fprintf(stderr, "ERR: sending event failed\n");
        } else {
            elosMessage_t *message = (elosMessage_t *)buffer;

            result = _getEventQueueIdFromJsonString(message->json);
        }

        free(sRequest);
    }

    fprintf(stdout, "\n");

    return result;
}

static int _eventUnsubscribe(int socket, char *eventQueue, char *buffer, size_t buflen) {
    char *sRequest = NULL;
    int result;

    const uint32_t eventQueueId = strtol(eventQueue, NULL, 10);

    fprintf(stdout, "unsubscribing from event %d...\n", eventQueueId);

    result = asprintf(&sRequest, "{\"eventQueueId\": \"%d\"}", eventQueueId);
    if (result < 0) {
        fprintf(stderr, "ERR: request string creation failed\n");
    } else {
        result = _sendElosEvent(socket, sRequest, buffer, buflen, ELOS_MESSAGE_EVENT_UNSUBSCRIBE);
        if (result < 0) {
            fprintf(stderr, "ERR: sending event failed\n");
        } else {
            printf("Buffer: %.*s\n", (int)buflen, buffer);
        }

        free(sRequest);
    }

    fprintf(stdout, "\n");

    return result;
}

static int _eventPublish(int socket, char *jsonString, char *buffer, size_t buflen) {
    int result;

    fprintf(stdout, "publishing event...\n");
    result = _sendElosEvent(socket, jsonString, buffer, buflen, ELOS_MESSAGE_EVENT_PUBLISH);
    fprintf(stdout, "\n");

    return result;
}

static int _tcpConnect(const char *host, int port) {
    int socketFd;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        fprintf(stderr, "ERR: create tcp socket\n");
    } else {
        /* setup sockaddr struct for IPv4 with port PORT */
        int retVal;
        struct sockaddr_in serverAddr = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr.s_addr = inet_addr(host),
        };

        retVal = connect(socketFd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if (retVal < 0) {
            fprintf(stderr, "ERR: connect to port %d\n", port);
            close(socketFd);
            socketFd = -1;
        }
    }

    return socketFd;
}

static void _printUsage(FILE *stream, const char *const elosProgramName) {
    fprintf(stream,
            "Usage: %s [OPTION]...\n"
            "  -s <filter>      event subscribe\n"
            "  -u <eventQueue>  unsubscribe from event queue\n"
            "  -p <payload>     raw payload to be published\n"
            "  -H <Host-Ip>     set host ip address\n"
            "  -P <Port>        set host port\n"
            "  -h               print this help\n"
            "  -v               get version\n\n"
            "<filter>: event filter in RPN notation\n"
            "         example: '.event.messageCode 1000 GE .event.messageCode 2000 LT AND'\n\n",
            elosProgramName);
}

static int _menu(int argc, char *argv[]) {
    char response[1500] = {0};
    int result = EXIT_FAILURE;
    int socketFd = -1;
    char *host = "127.0.0.1";
    int port = 54321;
    int c;

    while ((c = getopt(argc, argv, "s:u:p:H:P:vh")) != -1) {
        switch (c) {
            case 's':
                socketFd = _tcpConnect(host, port);
                if (socketFd >= 0) {
                    result = _eventSubscribe(socketFd, optarg, response, sizeof(response));
                    close(socketFd);
                    result = EXIT_SUCCESS;
                }
                break;
            case 'u':
                socketFd = _tcpConnect(host, port);
                if (socketFd >= 0) {
                    result = _eventUnsubscribe(socketFd, optarg, response, sizeof(response));
                    close(socketFd);
                    result = EXIT_SUCCESS;
                }
                break;
            case 'p':
                socketFd = _tcpConnect(host, port);
                if (socketFd >= 0) {
                    result = _eventPublish(socketFd, optarg, response, sizeof(response));
                    close(socketFd);
                    result = EXIT_SUCCESS;
                }
                break;
            case 'H':
                host = optarg;
                break;
            case 'P':
                port = strtol(optarg, NULL, 10);
                break;
            case 'v':
                socketFd = _tcpConnect(host, port);
                if (socketFd >= 0) {
                    result = _getVersion(socketFd, response, sizeof(response));
                    close(socketFd);
                    result = EXIT_SUCCESS;
                }
                break;
            case 'h':
                _printUsage(stdout, argv[0]);
                result = EXIT_SUCCESS;
                break;
            default:
                _printUsage(stderr, argv[0]);
                break;
        }
    }

    return result;
}

int main(int argc, char **argv) {
    int result = EXIT_FAILURE;

    result = _menu(argc, argv);

    return result;
}
