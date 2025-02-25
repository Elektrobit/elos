// SPDX-License-Identifier: MIT
#include <elos/libelosplugin/libelosplugin.h>
#include <elos/libelosplugin/types.h>
#include <errno.h>
#include <poll.h>
#include <regex.h>
#include <safu/common.h>
#include <safu/result.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifdef ELOSD_SYSTEMD
#include <systemd/sd-daemon.h>
#endif

#include "elos/event/event.h"
#include "elos/event/event_source.h"
#include "logline_mapper.h"
#include "safu/log.h"

#ifndef ELOSD_SYSLOG_PATH
#define ELOSD_SYSLOG_PATH "/dev/log"
#endif

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
    uint32_t status;
    elosLoglineMapper_t mapper;
    const samconfConfig_t *config;
    struct elosPublisher *publisher;
#ifdef ELOSD_SYSTEMD
    bool startedViaSocketActivation;
#endif
};

static safuResultE_t _setupSocket(elosPlugin_t *plugin);
static void _publishMessage(elosPlugin_t *plugin);

static safuResultE_t _pluginInit(elosPlugin_t *plugin) {
    safuLogDebugF("Starting to initialize '%s' plugin", plugin->config->key);
    struct syslog_context *context = (struct syslog_context *)malloc(sizeof(struct syslog_context));
    if (context == NULL) {
        return SAFU_RESULT_FAILED;
    }

    context->status = 0;
    context->socket = -1;
    context->cmd = -1;
    context->config = plugin->config;

    plugin->data = context;

    context->cmd = eventfd(0, 0);
    if (context->cmd == -1) {
        safuLogErrErrno("failed to establish eventfd for command channel");
        return SAFU_RESULT_FAILED;
    }

    if (elosLoglineMapperInit(&context->mapper, plugin->config) == SAFU_RESULT_FAILED) {
        safuLogErr("Failed to setup logline mapper");
        return SAFU_RESULT_FAILED;
    } else {
        context->status |= LOGLINE_MAPPER_NEEDS_CLEANUP;
    }

    elosPluginCreatePublisher(plugin, &context->publisher);

    return _setupSocket(plugin);
}

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            result = _pluginInit(plugin);
            safuLogDebugF("Scanner Plugin '%s' has been loaded", plugin->config->key);
        }
    }
    return result;
}

static safuResultE_t _pluginRunLoop(elosPlugin_t *plugin) {
    struct syslog_context *context = plugin->data;
    struct pollfd fds[] = {
        {.fd = context->socket, .events = POLLIN},
        {.fd = context->cmd, .events = POLLIN},
    };

    context->running = true;
    while (context->running) {
        int result = poll(fds, ARRAY_SIZE(fds), -1);
        if (result > 0) {
            if (fds[0].revents & POLLIN) {
                _publishMessage(plugin);
            } else if (fds[1].revents & POLLIN) {
                uint64_t command = 0;
                result = read(context->cmd, &command, sizeof(command));
                if (result != sizeof(command)) {
                    safuLogErrErrno("failed to receive command event");
                    return SAFU_RESULT_FAILED;
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
            return SAFU_RESULT_FAILED;
        }
    }
    return SAFU_RESULT_OK;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Scanner Plugin '%s' has been started", plugin->config->key);
        result = elosPluginReportAsStarted(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginReportAsStarted failed");
        } else {
            result = _pluginRunLoop(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErrF("Scanner loop failed for %s", plugin->config->key);
            }
            result = elosPluginStopTriggerWait(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosPluginStopTriggerWait failed");
            }
        }
    }
    return result;
}

static safuResultE_t _pluginStopScannerLoop(elosPlugin_t *plugin) {
    struct syslog_context *context = plugin->data;
    ssize_t result = 0;
    result = write(context->cmd, &elosScannerCmdStop, sizeof(elosScannerCommand_t));
    if (result == -1) {
        safuLogErrErrno("failed to send command event");
        return SAFU_RESULT_FAILED;
    }
    return SAFU_RESULT_OK;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Stopping Scanner Plugin '%s'", plugin->config->key);
        result = _pluginStopScannerLoop(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("Stopping Scanner Plugin '%s' failed", plugin->config->key);
        }
        result = elosPluginStopTriggerWrite(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginStopTriggerWrite failed");
        }
    }

    return result;
}

static safuResultE_t _freePluginResources(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    struct syslog_context *context = (struct syslog_context *)plugin->data;

    if (context->status & LOGLINE_MAPPER_NEEDS_CLEANUP) {
        elosLoglineMapperDeleteMembers(&context->mapper);
    }
    if (context->socket > -1) {
        if (close(context->socket) != 0) {
            safuLogErrErrno("Failed to close syslog socket");
            result = SAFU_RESULT_FAILED;
        }
    }
    if (context->cmd > -1) {
        if (close(context->cmd) != 0) {
            safuLogErrErrno("Failed to close syslog cmd socket");
            result = SAFU_RESULT_FAILED;
        }
    }
#ifdef ELOSD_SYSTEMD
    if (!context->startedViaSocketActivation) {
#else
    {
#endif
        // TODO decide how to handle environment variables in pluggins
        // and move the call to getEnv to be handled by samconf in a merge strategy
        const char *defaultSyslogPath =
            samconfConfigGetStringOr(plugin->config, "Config/SyslogPath", ELOSD_SYSLOG_PATH);
        const char *syslogPath = safuGetEnvOr("ELOS_SYSLOG_PATH", defaultSyslogPath);
        if (unlink(syslogPath) != 0) {
            safuLogErrErrno("Failed to unlink syslog socket");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (elosPluginDeletePublisher(plugin, context->publisher) != SAFU_RESULT_OK) {
        result = SAFU_RESULT_FAILED;
    }

    free(plugin->data);
    safuLogDebugF("Scanner Plugin '%s' resources freed", plugin->config->key);
    return result;
}

static safuResultE_t _pluginUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading Scanner Plugin '%s'", plugin->config->key);
        result = _freePluginResources(plugin);
    }

    return result;
}

static bool _socketActivationRequested(elosPlugin_t *plugin) {
    const char *env = safuGetEnvOr("ELOS_SYSLOG_USE_SYSTEMD_SOCKET", NULL);
    bool result = false;
    if (env == NULL) {
        bool useSystemdSocket = samconfConfigGetBoolOr(plugin->config, "Config/UseSystemdSocket", false);
        result = useSystemdSocket;
    } else if (strcmp(env, "1") == 0) {
        result = true;
    }
    return result;
}

#ifdef ELOSD_SYSTEMD
static safuResultE_t _setupSocketFromSystemdListenFds(elosPlugin_t *plugin) {
    struct syslog_context *context = plugin->data;
    int nSockets = sd_listen_fds(0);
    safuResultE_t result;
    if (nSockets == 1) {
        safuLogDebug("Started via systemd socket activation! Using passed socket.");
        context->socket = SD_LISTEN_FDS_START;
        context->startedViaSocketActivation = true;
        result = SAFU_RESULT_OK;
    } else if (nSockets > 1) {
        safuLogErr("Received too many sockets from systemd!");
        result = SAFU_RESULT_FAILED;
    } else {
        safuLogErr("Socket activation requested, but no socket received.");
        result = SAFU_RESULT_FAILED;
    }
    return result;
}
#endif

static safuResultE_t _setupSocket(elosPlugin_t *plugin) {
    struct syslog_context *context = plugin->data;
    struct sockaddr_un name = {.sun_family = AF_UNIX};

#ifdef ELOSD_SYSTEMD
    if (_socketActivationRequested(plugin)) {
        return _setupSocketFromSystemdListenFds(plugin);
    }
    context->startedViaSocketActivation = false;
#else
    if (_socketActivationRequested(plugin)) {
        safuLogWarn("Socket activation requested, but systemd support is disabled in this build. Ignoring.");
    }
#endif
    // TODO: move handlying of getEnv into samconf with an apropriate merge strategy
    const char *defaultSyslogPath = samconfConfigGetStringOr(plugin->config, "Config/SyslogPath", ELOSD_SYSLOG_PATH);
    const char *syslogPath = safuGetEnvOr("ELOS_SYSLOG_PATH", defaultSyslogPath);

    safuLogDebugF("SocketPath: %s", syslogPath);
    strncpy(name.sun_path, syslogPath, sizeof(name.sun_path) - 1);

    context->socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (context->socket == -1) {
        safuLogErrErrno("failed to setup syslog socket");
        return SAFU_RESULT_FAILED;
    }
    if (access(syslogPath, F_OK) == 0 && unlink(syslogPath) == -1) {
        safuLogErrErrno("failed to delete socket");
    }

    int result = bind(context->socket, (const struct sockaddr *)&name, sizeof(name));
    if (result == -1) {
        safuLogErrF("failed to bind socket to '%s' - %s", syslogPath, strerror(errno));
        return SAFU_RESULT_FAILED;
    }
    return SAFU_RESULT_OK;
}

static void _publishMessage(elosPlugin_t *plugin) {
    struct syslog_context *context = plugin->data;
    char *buffer = calloc(MAX_LOG_ENTRY_SIZE, sizeof(char));
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

    if (publish == true) {
        safuResultE_t result;
        elosEvent_t event = {0};
        elosLoglineMapperDoMapping(&context->mapper, &event, buffer);

        result = elosPluginPublish(plugin, context->publisher, &event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("eventPublish failed");
        }

        result = elosPluginStore(plugin, &event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("eventLog failed");
        }

        elosEventDeleteMembers(&event);
    }

    free(buffer);
}

elosPluginConfig_t elosPluginConfig = {
    .type = PLUGIN_TYPE_SCANNER,
    .load = _pluginLoad,
    .unload = _pluginUnload,
    .start = _pluginStart,
    .stop = _pluginStop,
};
