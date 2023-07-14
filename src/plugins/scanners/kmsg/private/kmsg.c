// SPDX-License-Identifier: MIT
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <safu/common.h>
#include <samconf/samconf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "elos/config/config.h"
#include "elos/event/event.h"
#include "elos/event/event_message_codes.h"
#include "elos/scanner/scanner.h"
#include "kmsg_mapper.h"
#include "safu/log.h"

#ifndef ELOS_KMSG_FILE
#define ELOS_KMSG_FILE "/dev/kmsg"
#endif

#define MAX_LOG_ENTRY_SIZE        4096
#define SCANNER_NAME              "kmsg"
#define SCANNER_CMD_STOP          1
#define SCANNER_KMSG_FILE_CREATED 1

typedef uint64_t scanner_command_t;

struct kmsg_context {
    uint32_t kmsgEventId;
    char *kmsgFile;
    int kmsgPollFd;
    int cmdPollFd;
    bool running;
    int flags;
    elosKmsgMapper_t mapper;
    const samconfConfig_t *config;
};

static elosScannerResultE_t _openKmsgFile(struct kmsg_context *context) {
    elosScannerResultE_t result = SCANNER_OK;
    struct stat stbuf = {0};
    int retval;
    const char *kmsgFile = elosConfigGetElosdScannerKmsgFile(context->config);

    context->kmsgFile = strdup(kmsgFile);

    retval = stat(kmsgFile, &stbuf);
    if (retval < 0) {
        if (errno == ENOENT) {
            mode_t mode = S_IRUSR | S_IWUSR;
            retval = mkfifo(kmsgFile, mode);
            if (retval < 0) {
                safuLogErrF("mkfifo '%s' failed - %s", kmsgFile, strerror(errno));
                result = SCANNER_ERROR;
            } else {
                context->flags |= SCANNER_KMSG_FILE_CREATED;
            }
        } else {
            safuLogErrF("stat '%s' failed - %s", kmsgFile, strerror(errno));
            result = SCANNER_ERROR;
        }
    } else if ((S_ISCHR(stbuf.st_mode) == 0) && (S_ISFIFO(stbuf.st_mode) == 0)) {
        safuLogErrF("file '%s' is neither S_IFCHR or S_IFIFO", kmsgFile);
        result = SCANNER_ERROR;
    }

    if (result == SCANNER_OK) {
        context->kmsgPollFd = open(kmsgFile, O_RDONLY | O_NONBLOCK);
        if (context->kmsgPollFd < 0) {
            safuLogErrF("open '%s' failed (%s)", kmsgFile, strerror(errno));
            result = SCANNER_ERROR;
        } else if ((context->flags & SCANNER_KMSG_FILE_CREATED) == 0) {
            lseek(context->kmsgPollFd, 0, SEEK_END);
        }
    }

    return result;
}

static elosScannerResultE_t _publishMessage(elosScannerSession_t *session) {
    struct kmsg_context *context = session->context;
    elosScannerResultE_t result = SCANNER_OK;
    char *readBuffer;
    int readBytes;

    readBuffer = calloc(sizeof(char), MAX_LOG_ENTRY_SIZE);
    if (readBuffer == NULL) {
        result = SCANNER_ERROR;
    }

    if (result == SCANNER_OK) {
        readBytes = read(context->kmsgPollFd, readBuffer, MAX_LOG_ENTRY_SIZE);
        if (readBytes < 0) {
            safuLogErrErrno("read message");
            result = SCANNER_ERROR;
        } else if ((readBytes > 0) && (readBuffer[readBytes - 1] == '\n')) {
            readBuffer[readBytes - 1] = '\0';
        } else {
            readBuffer[readBytes] = '\0';
        }
    }

    if (result == SCANNER_OK) {
        elosScannerCallbackData_t *cbData = &session->callback.scannerCallbackData;
        safuResultE_t resVal;

        elosEventSource_t eventSource = {.fileName = context->kmsgFile};
        elosEvent_t event = {
            .source = eventSource,
        };
        resVal = elosKmsgMapperDoMapping(&context->mapper, &event, readBuffer);
        if (resVal != SAFU_RESULT_OK) {
            event.date.tv_sec = time(NULL);
            event.date.tv_nsec = 0;
            event.messageCode = ELOS_MSG_CODE_MESSAGE_NOT_UNDERSTOOD;
            event.payload = readBuffer;
        }

        resVal = session->callback.eventPublish(cbData, &event);
        if (resVal != SAFU_RESULT_OK) {
            safuLogErr("eventPublish failed");
            result = SCANNER_ERROR;
        }

        resVal = session->callback.eventLog(cbData, &event);
        if (resVal != SAFU_RESULT_OK) {
            safuLogErr("eventLog failed");
            result = SCANNER_ERROR;
        }
    }

    free(readBuffer);

    return result;
}

elosScannerResultE_t elosScannerFree(elosScannerSession_t *session) {
    struct kmsg_context *context = session->context;
    if (context != NULL) {
        close(context->kmsgPollFd);
        close(context->cmdPollFd);

        if (context->kmsgFile != NULL) {
            if (context->flags & SCANNER_KMSG_FILE_CREATED) {
                unlink(context->kmsgFile);
            }
            free(context->kmsgFile);
        }

        free(session->context);
        session->context = NULL;
    }
    return SCANNER_OK;
}

elosScannerResultE_t elosScannerInitialize(elosScannerSession_t *session, const elosScannerParam_t *param) {
    struct kmsg_context *context = NULL;
    elosScannerResultE_t result = SCANNER_OK;

    context = (struct kmsg_context *)calloc(1, sizeof(struct kmsg_context));
    if (context == NULL) {
        result = SCANNER_ERROR;
    } else {
        context->kmsgPollFd = -1;
        context->cmdPollFd = -1;
        context->config = param->config;
        elosKmsgMapper_t mapper;
        if (elosKmsgMapperInit(&mapper) == SAFU_RESULT_OK) {
            context->mapper = mapper;
        } else {
            result = SCANNER_ERROR;
        }
    }

    if (result == SCANNER_OK) {
        context->cmdPollFd = eventfd(0, 0);
        if (context->cmdPollFd < 0) {
            safuLogErrErrno("failed to establish eventfd for command channel");
            result = SCANNER_ERROR;
        }
    }

    if (result == SCANNER_OK) {
        result = _openKmsgFile(context);
    }

    if (result != SCANNER_OK) {
        free(context);
    }

    if (result == SCANNER_OK) {
        session->name = SCANNER_NAME;
        session->context = context;
    }

    return result;
}

elosScannerResultE_t elosScannerRun(elosScannerSession_t *session) {
    elosScannerResultE_t result = SCANNER_OK;
    struct kmsg_context *context = session->context;
    struct pollfd fds[] = {
        {.fd = context->cmdPollFd, .events = POLLIN},
        {.fd = context->kmsgPollFd, .events = POLLIN},
    };

    context->running = true;
    while (context->running) {
        int retval;
        retval = poll(fds, ARRAY_SIZE(fds), -1);
        if (retval > 0) {
            if (fds[0].revents & POLLIN) {
                uint64_t command = 0;
                retval = read(context->cmdPollFd, &command, sizeof(command));
                if (retval != sizeof(command)) {
                    safuLogErrF("read failed on command event (%s)", strerror(errno));
                    context->running = false;
                    result = SCANNER_ERROR;
                    break;
                }
                if (command == SCANNER_CMD_STOP) {
                    context->running = false;
                    break;
                } else {
                    safuLogWarnF("ignore invalid command %lu\n", command);
                }
            }
            if (fds[1].revents & POLLIN) {
                retval = _publishMessage(session);
                if (retval < 0) {
                    safuLogErr("failed to publish message");
                    context->running = false;
                    result = SCANNER_ERROR;
                }
            }
        } else if (errno == EINTR) {
            continue;
        } else {
            safuLogErrF("polling failed - %s", strerror(errno));
            context->running = false;
            result = SCANNER_ERROR;
            break;
        }
    }

    return result;
}

elosScannerResultE_t elosScannerStop(elosScannerSession_t *session) {
    struct kmsg_context *context = session->context;
    const scanner_command_t command = SCANNER_CMD_STOP;
    elosScannerResultE_t result = SCANNER_OK;
    ssize_t retval = 0;

    retval = write(context->cmdPollFd, &command, sizeof(scanner_command_t));
    if (retval == -1) {
        safuLogErrErrno("failed to send command event");
        result = SCANNER_ERROR;
    }

    return result;
}
