// SPDX-License-Identifier: MIT
#include <bits/types/struct_timeval.h>
#include <safu/result.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/event/event_vector.h"
#include "elos/libelos/libelos.h"
#include "safu/common.h"
#include "safu/log.h"
#include "version.h"

typedef uint32_t elosEventId_t;

typedef enum {
    EVENT_SUBSCRIBE,
    EVENT_UNSUBSCRIBE,
    EVENT_PUBLISH,
    EVENT_LOG_FIND,
    GET_VERSION,
    UNKNOWN

} elosCommand_t;

typedef struct {
    char *host;
    int port;
    char *unixSocket;
    elosCommand_t command;
    char *commandArgs;
    size_t eventCount;
    size_t eventRate;
    elosEventQueueId_t eventQueueId;
    bool showUsage;
} elosConfig_t;

safuResultE_t elosInitConfig(int argc, char *argv[], elosConfig_t *config);
void elosDeleteConfig(elosConfig_t *config);
safuResultE_t elosExecuteCommand(elosSession_t *session, elosConfig_t *config);

static volatile bool elosSigintSeen = false;
void elosSigintHandler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        elosSigintSeen = true;
    }
}

void _printUsage(FILE *stream, const char *const elosProgramName) {
    fprintf(stream,
            "Usage: %s [OPTION]...\n"
            "  -s <filter>      event subscribe\n"
            "  -u <queueId>     event unsubscribe\n"
            "  -p <json event>  event publish\n"
            "  -f <filter>      event log find\n"
            "  -c <count>       set event count\n"
            "  -r <events/sec>  set event rate\n"
            "  -H <Host-Ip>     set host ip address\n"
            "  -P <Port>        set host port\n"
            "  -U <unix socket> set inix socket file name\n"
            "  -h               print this help\n"
            "  -v               get version\n\n"
            "<filter>: event filter in RPN notation\n"
            "         example: '.event.messageCode 1000 GE .event.messageCode 2000 LT AND'\n\n",
            elosProgramName);
}

int main(int argc, char *argv[]) {
    int result = EXIT_SUCCESS;
    safuResultE_t resVal = SAFU_RESULT_FAILED;
    elosConfig_t config = {0};

    // Improve smoke tests by disable message buffer
    setbuf(stdout, NULL);

    safuLogSetStreamLevel(SAFU_LOG_LEVEL_DEBUG);

    resVal = elosInitConfig(argc, argv, &config);
    if (resVal != SAFU_RESULT_OK) {
        fprintf(stderr, "ERROR: Parsing command line parameters failed!\n");
        _printUsage(stderr, argv[0]);
        result = EXIT_FAILURE;
    } else if (config.showUsage) {
        _printUsage(stdout, argv[0]);
    } else if (config.command == GET_VERSION) {
        elosPrintVersion(stdout, argv[0]);
        fprintf(stdout, "\n");
    } else {
        elosSession_t *session = NULL;
        const char *version = NULL;

        printf("Connecting to elosd...\n");
        resVal = elosConnectUnix(config.unixSocket, &session);
        if (resVal != SAFU_RESULT_OK) {
            printf("Cannot connect via unix socket, try with other available interface\n");
            resVal = elosConnectTcpip(config.host, config.port, &session);
            if (resVal != SAFU_RESULT_OK) {
                fprintf(stderr, "ERROR: Connection to elosd failed!\n");
                result = EXIT_FAILURE;
            }
        }

        if (resVal == SAFU_RESULT_OK) {
            printf("Get elosd version...\n");
            resVal = elosGetVersion(session, &version);
            if (resVal != SAFU_RESULT_OK) {
                fprintf(stderr, "ERROR: Fetching elosd version failed!\n");
                result = EXIT_FAILURE;
            } else {
                printf("  Elosd version is '%s'\n", version);
            }

            if (result == EXIT_SUCCESS) {
                resVal = elosExecuteCommand(session, &config);
                if (resVal != SAFU_RESULT_OK) {
                    fprintf(stderr, "ERROR: Executing command failed!\n");
                    result = EXIT_FAILURE;
                }
            }

            printf("Disconnecting from elosd...\n");
            resVal = elosDisconnect(session);
            if (resVal != SAFU_RESULT_OK) {
                fprintf(stderr, "ERROR: Disconnecting from elosd failed!\n");
                result = EXIT_FAILURE;
            }
        }
    }

    elosDeleteConfig(&config);

    return result;
}

safuResultE_t elosInitConfig(int argc, char *argv[], elosConfig_t *config) {
    safuResultE_t result = SAFU_RESULT_OK;
    uint32_t commands = 0;
    int c;

    config->showUsage = false;
    config->eventCount = 0;
    config->eventRate = 0;
    config->commandArgs = NULL;
    config->command = UNKNOWN;
    config->unixSocket = strdup("/run/elosd/elsod.socket");
    config->host = strdup("127.0.0.1");
    config->port = 54321;

    while (result == SAFU_RESULT_OK) {
        c = getopt(argc, argv, "s:u:p:f:c:r:H:P:U:vh");
        if (c == -1) {
            break;
        }

        switch (c) {
            case 's':
                config->command = EVENT_SUBSCRIBE;
                free(config->commandArgs);
                config->commandArgs = strdup(optarg);
                commands += 1;
                break;
            case 'u':
                config->command = EVENT_UNSUBSCRIBE;
                free(config->commandArgs);
                config->commandArgs = strdup(optarg);
                commands += 1;
                break;
            case 'p':
                config->command = EVENT_PUBLISH;
                free(config->commandArgs);
                config->commandArgs = strdup(optarg);
                commands += 1;
                break;
            case 'f':
                config->command = EVENT_LOG_FIND;
                free(config->commandArgs);
                config->commandArgs = strdup(optarg);
                commands += 1;
                break;
            case 'c':
                config->eventCount = strtoul(optarg, NULL, 10);
                break;
            case 'r':
                config->eventRate = strtoul(optarg, NULL, 10);
                break;
            case 'H':
                free(config->host);
                config->host = strdup(optarg);
                break;
            case 'P':
                config->port = strtol(optarg, NULL, 10);
                break;
            case 'U':
                free(config->unixSocket);
                config->unixSocket = strdup(optarg);
                break;
            case 'v':
                config->command = GET_VERSION;
                break;
            case 'h':
                config->showUsage = true;
                break;
            default:
                fprintf(stderr, "ERROR: Unrecognized option: '-%c'\n", optopt);
                result = SAFU_RESULT_FAILED;
                break;
        }
    }

    if (result == SAFU_RESULT_OK && config->showUsage == false) {
        if (config->command == UNKNOWN) {
            fprintf(stderr, "ERROR: elosc needs one of the following parameters: -s, -p or -f\n");
            result = SAFU_RESULT_FAILED;
        } else if (commands > 1) {
            fprintf(stderr, "ERROR: -s, -p and -f are mutually exclusive\n");
            result = SAFU_RESULT_FAILED;
        } else if ((config->command == EVENT_PUBLISH) && (config->eventCount == 0)) {
            config->eventCount = 1;
        }
    }

    return result;
}

void elosDeleteConfig(elosConfig_t *config) {
    free(config->commandArgs);
    free(config->host);
    free(config->unixSocket);
}

static inline safuResultE_t _commandEventSubscribeHelperPrintVector(elosEventVector_t *eventVector) {
    safuResultE_t result = SAFU_RESULT_OK;
    uint32_t elements;
    int retVal;

    elements = safuVecElements(eventVector);

    if (elements > 0) {
        struct timespec now = {0};

        retVal = clock_gettime(CLOCK_REALTIME, &now);
        if (retVal < 0) {
            fprintf(stderr, "reading the current time failed\n");
            result = SAFU_RESULT_FAILED;
        } else {
            char *sEventVector = NULL;

            result = elosEventVectorSerialize(eventVector, &sEventVector);
            if (result != SAFU_RESULT_OK) {
                fprintf(stderr, "Failed to serialize events");
            } else {
                printf("  new data [%lu,%lu]:\n%s\n", now.tv_sec, now.tv_nsec, sEventVector);
            }

            free(sEventVector);
        }
    }

    return result;
}

static safuResultE_t _commandEventSubscribe(elosSession_t *session, elosConfig_t *config) {
    char const *filterArray[] = {config->commandArgs};
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventVector_t *eventVector = NULL;

    signal(SIGINT, elosSigintHandler);
    signal(SIGTERM, elosSigintHandler);

    printf("subscribing with filterRule '%s'\n", filterArray[0]);
    result = elosEventSubscribe(session, filterArray, ARRAY_SIZE(filterArray), &config->eventQueueId);
    if (result != SAFU_RESULT_OK) {
        fprintf(stderr, "event subscription failed\n");
    } else {
        printf("successfully subscribed to event queue %d\n", config->eventQueueId);
        printf("polling for new events...\n");
        while (elosSigintSeen == false) {
            result = elosEventQueueRead(session, config->eventQueueId, &eventVector);
            if (result != SAFU_RESULT_OK) {
                fprintf(stderr, "reading from eventqueue failed\n");
                break;
            } else {
                result = _commandEventSubscribeHelperPrintVector(eventVector);
                if (result != SAFU_RESULT_OK) {
                    fprintf(stderr, "reading from eventqueue failed\n");
                    break;
                } else if (config->eventRate != 0) {
                    usleep(1000000 / config->eventRate);
                } else {
                    sleep(1);
                }

                elosEventVectorDelete(eventVector);
                eventVector = NULL;
            }
        }
    }

    return result;
}

static safuResultE_t _commandEventUnsubscribe(elosSession_t *session, elosConfig_t *config) {
    safuResultE_t result;
    const uint32_t eventQueueId = strtol(config->commandArgs, NULL, 10);

    printf("unsubscribing from queueId '%d'\n", eventQueueId);
    result = elosEventUnsubscribe(session, eventQueueId);
    if (result != SAFU_RESULT_OK) {
        fprintf(stderr, "event unsubscription failed\n");
    }

    return result;
}

static safuResultE_t _commandEventPublish(elosSession_t *session, elosConfig_t *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEvent_t *event = NULL;

    printf("publishing event %lu times: '%s'\n", config->eventCount, config->commandArgs);
    result = elosEventNew(&event);
    if (result == SAFU_RESULT_FAILED) {
        fprintf(stderr, "creating new event failed!\n");
    } else {
        result = elosEventDeserialize(event, config->commandArgs);
        if (result == SAFU_RESULT_FAILED) {
            fprintf(stderr, "deserializing event failed!\n");
        }
    }
    if (result == SAFU_RESULT_OK) {
        for (size_t i = 0; i < config->eventCount; i++) {
            result = elosEventPublish(session, event);
            if (result != SAFU_RESULT_OK) {
                fprintf(stderr, "publishing event failed!\n");
                break;
            } else if (config->eventRate != 0) {
                usleep(1000000 / config->eventRate);
            }
        }
        elosEventDelete(event);
    }

    return result;
}

static safuResultE_t _commandLogFindEvent(elosSession_t *session, elosConfig_t *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEventVector_t *eventVector = NULL;
    int retVal;

    struct timespec newest = {0};
    struct timespec oldest = {0};
    retVal = elosFindEvents(session, config->commandArgs, &newest, &oldest, &eventVector);
    if (retVal < 0) {
        fprintf(stderr, "event find failed!\n");
    } else if (eventVector == NULL) {
        printf("No matching events returned\n");
        result = SAFU_RESULT_OK;
    } else {
        for (uint32_t i = 0; i < eventVector->elementCount; i++) {
            elosEvent_t *event;

            event = (elosEvent_t *)safuVecGet(eventVector, i);
            if (event == NULL) {
                fprintf(stderr, "reading from eventVector failed!\n");
                result = SAFU_RESULT_FAILED;
                break;
            } else {
                char *eventsAsJsonString = NULL;
                result = elosEventSerialize(&eventsAsJsonString, event);
                if (result == SAFU_RESULT_FAILED) {
                    fprintf(stderr, "serializing event from eventVector failed!\n");
                    break;
                } else {
                    printf("  new data:\n%s\n", eventsAsJsonString);
                }

                free(eventsAsJsonString);
            }
        }

        elosEventVectorDelete(eventVector);
        eventVector = NULL;
    }

    return result;
}

safuResultE_t elosExecuteCommand(elosSession_t *session, elosConfig_t *config) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    switch (config->command) {
        case EVENT_SUBSCRIBE:
            result = _commandEventSubscribe(session, config);
            break;
        case EVENT_UNSUBSCRIBE:
            result = _commandEventUnsubscribe(session, config);
            break;
        case EVENT_PUBLISH:
            result = _commandEventPublish(session, config);
            break;
        case EVENT_LOG_FIND:
            result = _commandLogFindEvent(session, config);
            break;
        default:
            printf("Unknown command\n");
            break;
    }

    return result;
}
