// SPDX-License-Identifier: MIT
#include <errno.h>
#include <poll.h>
#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "elos/config/config.h"
#include "elos/event/event.h"
#include "elos/event/event_source.h"
#include "elos/scanner/scanner.h"
#include "logline_mapper.h"
#include "safu/log.h"

#ifndef ELOSD_SYSLOG_PATH
#define ELOSD_SYSLOG_PATH "/dev/log"
#endif

#define REGEX_FILTER_ACTIVE          0x01
#define REGEX_FILTER_NEEDS_CLEANUP   0x02
#define LOGLINE_MAPPER_NEEDS_CLEANUP (0x01 << 2)
#define MAX_LOG_ENTRY_SIZE           4096
#define SCANNER_NAME                 "syslog"

typedef uint64_t elosScannerCommand_t;
static const elosScannerCommand_t elosScannerCmdStop = 1;

struct syslog_context {
    uint32_t syslogEventId;
    int socket;
    int cmd;
    bool running;
    regex_t regExFilter;
    uint32_t status;
    elosLoglineMapper_t mapper;
    const samconfConfig_t *config;
};

static elosScannerResultE_t _setupSocket(struct syslog_context *context);
static void _publishMessage(elosScannerSession_t *session);

elosScannerResultE_t elosScannerInitialize(elosScannerSession_t *session, const elosScannerParam_t *param) {
    const char *filterString = NULL;
    const char *configPath = "/root/elos/Scanner/SyslogScanner";
    struct syslog_context *context = (struct syslog_context *)malloc(sizeof(struct syslog_context));
    if (context == NULL) {
        return SCANNER_ERROR;
    }

    session->name = SCANNER_NAME;
    session->context = context;
    context->status = 0;
    context->socket = -1;
    context->cmd = -1;
    context->config = param->config;

    if (param != NULL && context->config != NULL) {
        if (samconfConfigGet(context->config, configPath, &session->config) != SAMCONF_CONFIG_OK) {
            safuLogInfo("No config for syslog scanner found");
        }
    }

    context->cmd = eventfd(0, 0);
    if (context->cmd == -1) {
        safuLogErrErrno("failed to establish eventfd for command channel");
        return SCANNER_ERROR;
    }

    if (samconfConfigGetString(session->config, "FilterString", &filterString) == SAMCONF_CONFIG_OK) {
        int retval;
        retval = regcomp(&context->regExFilter, filterString, 0);
        if (retval != 0) {
            safuLogErrF("regcomp for filterString '%s' failed!", filterString);
        } else {
            // There seems to be no easy way to detect if a regex_t is valid;
            // This causes problems further down the line as regfree() causes
            // segmentation faults when used on a uninitialized regex_t struct,
            // so we have to rely on flags to make sure everything runs smoothly
            context->status |= (REGEX_FILTER_ACTIVE | REGEX_FILTER_NEEDS_CLEANUP);
        }
    }

    if (elosLoglineMapperInit(&context->mapper, session->config) == SAFU_RESULT_FAILED) {
        safuLogErr("Failed to setup logline mapper");
        return SCANNER_ERROR;
    } else {
        context->status |= LOGLINE_MAPPER_NEEDS_CLEANUP;
    }

    return _setupSocket(context);
}

elosScannerResultE_t elosScannerRun(elosScannerSession_t *session) {
    struct syslog_context *context = session->context;
    struct pollfd fds[] = {
        {.fd = context->socket, .events = POLLIN},
        {.fd = context->cmd, .events = POLLIN},
    };

    context->running = true;
    while (context->running) {
        int result = poll(fds, ARRAY_SIZE(fds), -1);
        if (result > 0) {
            if (fds[0].revents & POLLIN) {
                _publishMessage(session);
            } else if (fds[1].revents & POLLIN) {
                uint64_t command = 0;
                result = read(context->cmd, &command, sizeof(command));
                if (result != sizeof(command)) {
                    safuLogErrErrno("failed to receive command event");
                    return SCANNER_ERROR;
                }
                if (command == elosScannerCmdStop) {
                    context->running = false;
                    break;
                } else {
                    safuLogInfoF("Ignore command %lu\n", command);
                }
            }
        } else if (errno == EINTR) {
            continue;
        } else {
            safuLogErrErrno("polling failed, bail out");
            return SCANNER_ERROR;
        }
    }

    return SCANNER_OK;
}

elosScannerResultE_t elosScannerStop(elosScannerSession_t *session) {
    struct syslog_context *context = session->context;
    ssize_t result = 0;
    result = write(context->cmd, &elosScannerCmdStop, sizeof(elosScannerCommand_t));
    if (result == -1) {
        safuLogErrErrno("failed to send command event");
        return SCANNER_ERROR;
    }
    return SCANNER_OK;
}

elosScannerResultE_t elosScannerFree(elosScannerSession_t *session) {
    struct syslog_context *context = session->context;
    if (context->status & REGEX_FILTER_NEEDS_CLEANUP) {
        regfree(&context->regExFilter);
    }

    if (context->status & LOGLINE_MAPPER_NEEDS_CLEANUP) {
        elosLoglineMapperDeleteMembers(&context->mapper);
    }

    if (context->socket > -1) {
        close(context->socket);
    }

    if (context->socket > -1) {
        close(context->cmd);
    }

    unlink(elosConfigGetElosdSyslogSocketPath(session->config));
    free(session->context);
    return SCANNER_OK;
}

static elosScannerResultE_t _setupSocket(struct syslog_context *context) {
    struct sockaddr_un name = {.sun_family = AF_UNIX};
    const char *syslogSocketPath = elosConfigGetElosdSyslogSocketPath(context->config);
    strncpy(name.sun_path, syslogSocketPath, sizeof(name.sun_path) - 1);

    context->socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (context->socket == -1) {
        safuLogErrErrno("failed to setup syslog socket");
        return SCANNER_ERROR;
    }
    if (access(syslogSocketPath, F_OK) == 0 && unlink(syslogSocketPath) == -1) {
        safuLogErrErrno("failed to delete socket");
    }

    int result = bind(context->socket, (const struct sockaddr *)&name, sizeof(name));
    if (result == -1) {
        safuLogErrF("failed to bind socket to '%s' - %s", syslogSocketPath, strerror(errno));
        return SCANNER_ERROR;
    }
    return SCANNER_OK;
}

static void _publishMessage(elosScannerSession_t *session) {
    const char *filterString = NULL;
    struct syslog_context *context = session->context;
    char *buffer = calloc(sizeof(char), MAX_LOG_ENTRY_SIZE);
    bool publish = true;

    int bytesRead = read(context->socket, buffer, MAX_LOG_ENTRY_SIZE);
    if (bytesRead == -1) {
        safuLogErrErrno("read message");
        free(buffer);
        return;
    }

    if ((bytesRead != 0) && (buffer[bytesRead - 1] == '\n')) {
        buffer[bytesRead - 1] = '\0';
    } else {
        buffer[bytesRead] = '\0';
    }

    if (context->status & REGEX_FILTER_ACTIVE) {
        int retval;
        retval = regexec(&context->regExFilter, buffer, 0, NULL, 0);
        if (retval == REG_NOMATCH) {
            publish = false;
        } else if (retval != 0) {
            samconfConfigGetString(session->config, "FilterString", &filterString);
            safuLogErrF("regexec for filterString '%s' failed! Disabling filter.", filterString);
            context->status ^= REGEX_FILTER_ACTIVE;
        }
    }

    if (publish == true) {
        elosScannerCallbackData_t *cbData = &session->callback.scannerCallbackData;
        safuResultE_t result;
        elosEvent_t event = {0};
        elosLoglineMapperDoMapping(&context->mapper, &event, buffer);

        result = session->callback.eventLog(cbData, &event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("eventPublish failed");
        }

        result = session->callback.eventPublish(cbData, &event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("eventLog failed");
        }

        elosEventDeleteMembers(&event);
    }

    free(buffer);
}
