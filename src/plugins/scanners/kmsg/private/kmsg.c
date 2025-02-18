// SPDX-License-Identifier: MIT
#include <elos/event/event_message_codes.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <elos/libelosplugin/types.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/result.h>
#include <samconf/samconf.h>
#include <samconf/samconf_types.h>
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

#include "kmsg_mapper.h"
#include "safu/log.h"

#ifndef ELOS_KMSG_FILE
#define ELOS_KMSG_FILE "/dev/kmsg"
#endif

#ifndef ELOS_RUNDIR
#define ELOS_RUNDIR "/run/elosd"
#endif

#define MAX_LOG_ENTRY_SIZE 4096
#define SCANNER_NAME       "kmsg"
#define SCANNER_CMD_STOP   1

typedef uint64_t elosScannerCommand_t;

static const samconfConfig_t *_getElosRootConfig(const samconfConfig_t *pluginConfig) {
    bool isElosRootConfig = false;

    const samconfConfig_t *node = pluginConfig != NULL ? pluginConfig->parent : NULL;
    while (node != NULL) {
        isElosRootConfig = strcmp(node->key, "elos") == 0;
        if (isElosRootConfig == true) {
            break;
        } else {
            node = node->parent;
        }
    }

    return isElosRootConfig == true ? node : NULL;
}

static const char *_getElosRunDir(const samconfConfig_t *pluginConfig) {
    const char *runDir = NULL;

    runDir = safuGetEnvOr("ELOS_RUNDIR", NULL);
    if (runDir == NULL) {
        const samconfConfig_t *elosConfig = _getElosRootConfig(pluginConfig);
        if (elosConfig != NULL) {
            runDir = samconfConfigGetStringOr(elosConfig, "RunDir", ELOS_RUNDIR);
        } else {
            runDir = ELOS_RUNDIR;
        }
    }

    return runDir;
}

static char *_getDefaultKmsgStateFile(const samconfConfig_t *pluginConfig) {
    const char *runDir = _getElosRunDir(pluginConfig);

    ssize_t length = snprintf(NULL, 0, "%s/kmsg.state", runDir) + sizeof('\0');
    char *defaultKmsgStateFile = safuAllocMem(NULL, length);
    if (defaultKmsgStateFile != NULL) {
        snprintf(defaultKmsgStateFile, length, "%s/kmsg.state", runDir);
    }

    return defaultKmsgStateFile;
}

static char *_getKmsgStateFile(const samconfConfig_t *pluginConfig) {
    char *kmsgStateFile = NULL;

    const char *stateFileFormEnv = safuGetEnvOr("ELOS_KMSGSTATEFILE", NULL);
    if (stateFileFormEnv == NULL) {
        const char *stateFileFromConfig = samconfConfigGetStringOr(pluginConfig, "Config/KmsgStateFile", NULL);
        if (kmsgStateFile == NULL) {
            kmsgStateFile = _getDefaultKmsgStateFile(pluginConfig);
        } else {
            kmsgStateFile = strdup(stateFileFromConfig);
        }
    } else {
        kmsgStateFile = strdup(stateFileFormEnv);
    }

    return kmsgStateFile;
}

static safuResultE_t _touchFile(const char *file) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    int stateFd = open(file, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
    if (stateFd == -1) {
        safuLogErrErrnoValue("Failed to create kmsg state file", stateFd);
    } else {
        close(stateFd);
        result = SAFU_RESULT_OK;
    }

    return result;
}

static safuResultE_t _openKmsgFile(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    struct elosKmsgContext *context = plugin->data;
    struct stat stbuf = {0};
    int retval;

    char *kmsgStateFile = _getKmsgStateFile(plugin->config);
    if (kmsgStateFile == NULL) {
        safuLogErr("Failed to setup kmsg state file.");
    } else {
        safuLogDebugF("Save kmsg-State in %s", kmsgStateFile);

        const char *defaultKmsgFile = samconfConfigGetStringOr(plugin->config, "Config/KmsgFile", ELOS_KMSG_FILE);
        const char *kmsgFile = safuGetEnvOr("ELOS_KMSG_FILE", defaultKmsgFile);
        context->kmsgFile = strdup(kmsgFile);

        retval = stat(kmsgFile, &stbuf);
        if (retval < 0) {
            if (errno == ENOENT) {
                mode_t mode = S_IRUSR | S_IWUSR;
                retval = mkfifo(kmsgFile, mode);
                if (retval < 0) {
                    safuLogErrF("mkfifo '%s' failed - %s", kmsgFile, strerror(errno));
                    result = SAFU_RESULT_FAILED;
                } else {
                    context->flags |= SCANNER_KMSG_FILE_CREATED;
                }
            } else {
                safuLogErrF("stat '%s' failed - %s", kmsgFile, strerror(errno));
                result = SAFU_RESULT_FAILED;
            }
        } else if ((S_ISCHR(stbuf.st_mode) == 0) && (S_ISFIFO(stbuf.st_mode) == 0)) {
            safuLogErrF("file '%s' is neither S_IFCHR or S_IFIFO", kmsgFile);
            result = SAFU_RESULT_FAILED;
        }

        if (result == SAFU_RESULT_OK) {
            context->kmsgPollFd = open(kmsgFile, O_RDONLY | O_NONBLOCK);
            if (context->kmsgPollFd < 0) {
                safuLogErrF("open '%s' failed (%s)", kmsgFile, strerror(errno));
                result = SAFU_RESULT_FAILED;
            } else {
                bool skipBuffer = false;

                int fileExists = access(kmsgStateFile, F_OK);
                if (fileExists == 0) {
                    skipBuffer = true;
                } else {
                    switch (errno) {
                        case ENOENT:
                            result = _touchFile(kmsgStateFile);
                            if (result != SAFU_RESULT_OK) {
                                skipBuffer = true;
                            }
                            break;
                        default:
                            safuLogErrErrno("Failed to check kmsg.state file, skip kmsg buffer content.");
                            skipBuffer = true;
                    }
                }

                if (skipBuffer == true && (context->flags & SCANNER_KMSG_FILE_CREATED) == 0) {
                    lseek(context->kmsgPollFd, 0, SEEK_END);
                }
            }
        }
    }

    free(kmsgStateFile);
    return result;
}

static void _publishMessage(elosPlugin_t *plugin) {
    struct elosKmsgContext *context = plugin->data;
    safuResultE_t result = SAFU_RESULT_OK;
    char *readBuffer;
    int readBytes;

    readBuffer = calloc(MAX_LOG_ENTRY_SIZE, sizeof(char));
    if (readBuffer == NULL) {
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        readBytes = read(context->kmsgPollFd, readBuffer, MAX_LOG_ENTRY_SIZE);
        if (readBytes < 0) {
            safuLogErrErrno("read message");
            result = SAFU_RESULT_FAILED;
        } else if ((readBytes > 0) && (readBuffer[readBytes - 1] == '\n')) {
            readBuffer[readBytes - 1] = '\0';
        } else {
            readBuffer[readBytes] = '\0';
        }
    }

    if (result == SAFU_RESULT_OK) {
        elosEvent_t event = {
            .source =
                {
                    .fileName = context->kmsgFile,
                },
        };

        result = elosKmsgMapperDoMapping(&context->mapper, &event, readBuffer);
        if (result != SAFU_RESULT_OK) {
            event.date.tv_sec = time(NULL);
            event.date.tv_nsec = 0;
            event.messageCode = ELOS_MSG_CODE_MESSAGE_NOT_UNDERSTOOD;
            event.payload = readBuffer;
        }

        result = elosPluginPublish(plugin, context->publisher, &event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("eventPublish failed");
        }

        result = elosPluginStore(plugin, &event);
        if (result != SAFU_RESULT_OK) {
            safuLogErr("eventLog failed");
        }
    }

    free(readBuffer);
}

static safuResultE_t _freePluginResources(elosPlugin_t *plugin) {
    struct elosKmsgContext *context = plugin->data;
    safuResultE_t result = SAFU_RESULT_OK;
    if (context != NULL) {
        close(context->kmsgPollFd);
        close(context->cmdPollFd);

        if (context->kmsgFile != NULL) {
            if (context->flags & SCANNER_KMSG_FILE_CREATED) {
                unlink(context->kmsgFile);
            }
            free(context->kmsgFile);
        }

        if (elosPluginDeletePublisher(plugin, context->publisher) != SAFU_RESULT_OK) {
            result = SAFU_RESULT_FAILED;
        }

        free(plugin->data);
        plugin->data = NULL;
    }
    return result;
}

static safuResultE_t _pluginUnload(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Unloading %s Scanner Plugin '%s'", SCANNER_NAME, plugin->config->key);
        result = _freePluginResources(plugin);
    }

    return result;
}

static safuResultE_t _pluginInit(elosPlugin_t *plugin) {
    struct elosKmsgContext *context = NULL;
    safuResultE_t result = SAFU_RESULT_OK;

    context = (struct elosKmsgContext *)calloc(1, sizeof(struct elosKmsgContext));
    if (context == NULL) {
        result = SAFU_RESULT_FAILED;
    } else {
        context->kmsgPollFd = -1;
        context->cmdPollFd = -1;
        elosKmsgMapper_t mapper;
        if (elosKmsgMapperInit(&mapper) == SAFU_RESULT_OK) {
            context->mapper = mapper;
        } else {
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        context->cmdPollFd = eventfd(0, 0);
        if (context->cmdPollFd < 0) {
            safuLogErrErrno("failed to establish eventfd for command channel");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        plugin->data = context;
        result = _openKmsgFile(plugin);
    }

    if (result == SAFU_RESULT_OK) {
        result = elosPluginCreatePublisher(plugin, &context->publisher);
    } else {
        free(context);
        plugin->data = NULL;
    }

    return result;
}

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
            safuLogErr("Given configuration is NULL or has .key set to NULL");
        } else {
            plugin->data = NULL;
            result = _pluginInit(plugin);
            safuLogDebugF("%s Scanner Plugin '%s' has been loaded", SCANNER_NAME, plugin->config->key);
        }
    }

    return result;
}

static safuResultE_t _pluginRunLoop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_OK;
    struct elosKmsgContext *context = plugin->data;
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
                    result = SAFU_RESULT_FAILED;
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
                _publishMessage(plugin);
            }
        } else if (errno == EINTR) {
            continue;
        } else {
            safuLogErrF("polling failed - %s", strerror(errno));
            context->running = false;
            result = SAFU_RESULT_FAILED;
            break;
        }
    }

    return result;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("%s Scanner Plugin '%s' has been started", SCANNER_NAME, plugin->config->key);

        result = elosPluginReportAsStarted(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginReportAsStarted failed");
        } else {
            result = _pluginRunLoop(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErrF("scanner loop failed for %s", plugin->config->key);
            }
            result = elosPluginStopTriggerWait(plugin);
            if (result == SAFU_RESULT_FAILED) {
                safuLogErr("elosPluginStopTriggerWait failed");
            }
        }
    }

    return result;
}

static safuResultE_t _pluginStopLoop(elosPlugin_t *plugin) {
    struct elosKmsgContext *context = plugin->data;
    const elosScannerCommand_t command = SCANNER_CMD_STOP;
    safuResultE_t result = SAFU_RESULT_OK;
    ssize_t retval = 0;

    retval = write(context->cmdPollFd, &command, sizeof(elosScannerCommand_t));
    if (retval == -1) {
        safuLogErrErrno("failed to send command event");
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    if (plugin == NULL) {
        safuLogErr("Null parameter given");
    } else {
        safuLogDebugF("Stopping %s Scanner Plugin '%s'", SCANNER_NAME, plugin->config->key);
        result = _pluginStopLoop(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErrF("Stoping %s Scanner Plugin instance '%s' failed", SCANNER_NAME, plugin->config->key);
        }

        result = elosPluginStopTriggerWrite(plugin);
        if (result == SAFU_RESULT_FAILED) {
            safuLogErr("elosPluginStopTriggerWrite failed");
        }
    }

    return result;
}

elosPluginConfig_t elosPluginConfig = {
    .type = PLUGIN_TYPE_SCANNER,
    .load = _pluginLoad,
    .unload = _pluginUnload,
    .start = _pluginStart,
    .stop = _pluginStop,
};
