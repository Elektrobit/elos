// SPDX-License-Identifier: MIT

#include <linux/limits.h>
#include <stdint.h>
#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <ftw.h>
#include <limits.h>
#include <samconf/samconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/libelos/libelos.h"
#include "safu/common.h"
#include "safu/log.h"

#ifndef ELOS_COREDUMP_CONFIG_ROOT
#define ELOS_COREDUMP_CONFIG_ROOT "/root/elos/coredump/"
#endif

#define BUF_SIZE             1024
#define COREDUMP_TARGET_PATH "/tmp"
#define COREDUMP_TOPIC_ID    42

/*
 * expected order of specifiers:
 *
 *   %P %E %u %g %s %t %h
 *
 * where:
 *
 *   %P: PID of dumped process
 *   %E: Pathname of executable, with slashes ('/') replaced by exclamation marks ('!')
 *   %u: Numeric real UID of dumped process.
 *   %g: Numeric real GID of dumped process.
 *   %s: Number of signal causing dump.
 *   %t: Time of dump, expressed as seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *   %h: Hostname
 */
enum elosCoredumpSpecifiersOrderE {
    COREDUMP_PID = 1,
    COREDUMP_APP_PATH,
    COREDUMP_UID,
    COREDUMP_GID,
    COREDUMP_SIGNAL,
    COREDUMP_TIME,
    COREDUMP_HOSTNAME,
    COREDUMP_MAX_SPECIFIER, /* must be the last entry */
};

typedef struct elosCoredumpConfig {
    char *path;
    unsigned long allocatedMem;
} elosCoredumpConfig_t;

int elosPrepareCorePattern(void) {
    char executablePath[PATH_MAX];
    ssize_t count;
    FILE *fp;
    int uid;
    int nwrite;

    uid = (int)geteuid();
    if (uid != 0) {
        fprintf(stderr, "prepare_coredump not called as root and failed!\n");
        return -1;
    }

    count = readlink("/proc/self/exe", executablePath, PATH_MAX);
    if ((count == -1) || (count == PATH_MAX)) {
        fprintf(stderr, "prepare_coredump could not get its executable filepath and failed!\n");
        return -1;
    }

    fp = fopen("/proc/sys/kernel/core_pattern", "w");
    if (fp == NULL) {
        fprintf(stderr, "prepare_coredump could not open /proc/sys/kernel/core_pattern and failed!\n");
        return -1;
    }

    nwrite = fprintf(fp, "|%s %%P %%E %%u %%g %%s %%t %%h", executablePath);
    if (nwrite != (count + 22)) {
        fprintf(stderr, "prepare_coredump could not correctly write /proc/sys/kernel/core_pattern and failed!\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);

    fprintf(stdout, "coredump core_pattern setup done\n");

    return 0;
}

void _replaceCharsInString(char *str, uint32_t len, char old, char new) {
    uint32_t i = 0;

    while (str[i] != '\0' && i < len) {
        if (str[i] == old) str[i] = new;
        i++;
    }
}

safuResultE_t elosCoredumpConfigInit(elosCoredumpConfig_t *coredumpConfig) {
    const char *location = NULL;
    samconfConfig_t *config = NULL;
    int32_t relMem = 0;
    int32_t absMem = 0;
    unsigned long fileSystemSize = 0;
    const char *pathname = NULL;
    struct statvfs dirBuf = {0};
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    safuResultE_t result = SAFU_RESULT_OK;
    int ret = 0;

    fprintf(stdout, "loading coredump configuration\n");

    location = safuGetEnvOr("ELOS_COREDUMP_CONFIG_FILE", ELOS_COREDUMP_CONFIG_FILE);
    status = samconfLoad(location, false, &config);

    if (status != SAMCONF_CONFIG_OK) {
        fprintf(stdout, "loading coredump configuration failed \n");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        status = samconfConfigGetString(config, ELOS_COREDUMP_CONFIG_ROOT "coredumppath/", &pathname);
        if (status == SAMCONF_CONFIG_OK && pathname != NULL) {
            coredumpConfig->path = strdup(pathname);
            if (coredumpConfig->path != NULL) {
                ret = statvfs(coredumpConfig->path, &dirBuf);
                if (ret == 0) {
                    fileSystemSize = dirBuf.f_bsize * dirBuf.f_blocks;
                } else {
                    ret = mkdir(coredumpConfig->path, S_IRWXU);
                    if (ret != 0) {
                        safuLogErrErrno("Configuration coredump path does not exist and can not be created\n");
                        result = SAFU_RESULT_FAILED;
                    } else if (statvfs(coredumpConfig->path, &dirBuf) == 0) {
                        fileSystemSize = dirBuf.f_bsize * dirBuf.f_blocks;
                    } else {
                        safuLogErrErrno("Configuration coredump path created but cannot get size\n");
                        result = SAFU_RESULT_FAILED;
                    }
                }
            } else {
                fprintf(stderr, "copying path failed !");
                result = SAFU_RESULT_FAILED;
            }
        } else {
            fprintf(stderr, "coredump path is not set in config !");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        status = samconfConfigGetInt32(config, ELOS_COREDUMP_CONFIG_ROOT "totalmem_rel/", &relMem);
        if (status == SAMCONF_CONFIG_OK) {
            if (relMem > 0) {
                coredumpConfig->allocatedMem = ((fileSystemSize * relMem) / 100);
                fprintf(stdout, "coredump memory allocated : %ld\n", coredumpConfig->allocatedMem);
            }
        } else {
            status = samconfConfigGetInt32(config, ELOS_COREDUMP_CONFIG_ROOT "totalmem/", &absMem);
            if (status == SAMCONF_CONFIG_OK) {
                coredumpConfig->allocatedMem = absMem;
                fprintf(stdout, "coredump memory allocated : %ld\n", coredumpConfig->allocatedMem);
            } else {
                fprintf(stdout, "neither totalmem nor totalmem_rel is set, memory calculation failed !\n");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    samconfConfigDelete(config);
    return result;
}

void elosCoredumpConfigDelete(elosCoredumpConfig_t *coredumpConfig) {
    free(coredumpConfig->path);
}

int main(int argc, char *argv[]) {
    char *p;
    char buf[BUF_SIZE];
    char coredumpTargetFile[PATH_MAX];
    char applicationPath[PATH_MAX];
    char payloadEvent[PATH_MAX + 128];
    size_t nread;
    ssize_t total;
    FILE *fp;
    int ret;
    safuResultE_t result = SAFU_RESULT_OK;
    elosSession_t *session;
    elosCoredumpConfig_t coredumpConfig = {0};
    elosEvent_t event = {0};

    fprintf(stdout, "initializing coredump configuration\n");
    result = elosCoredumpConfigInit(&coredumpConfig);

    if (result != SAFU_RESULT_OK) {
        fprintf(stdout, "initializing coredump configuration failed \n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    fprintf(stdout, "coredump started\n");

    if (argc == 1) {
        fprintf(stdout, "coredump called without parameter - setup core_pattern\n");
        ret = elosPrepareCorePattern();
        elosCoredumpConfigDelete(&coredumpConfig);
        return ret;
    } else if (argc != COREDUMP_MAX_SPECIFIER) {
        fprintf(stderr, "coredump called with invalid specifiers and failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    ret = snprintf(applicationPath, PATH_MAX, "%s", argv[COREDUMP_APP_PATH]);
    if (ret >= PATH_MAX) {
        fprintf(stderr, "reading application path specifier failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }
    _replaceCharsInString(applicationPath, PATH_MAX, '/', '_');

    event.date.tv_sec = (time_t)strtoull(argv[COREDUMP_TIME], &p, 10);
    if (*p != 0) {
        fprintf(stderr, "reading timestamp specifier failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    event.source.pid = (pid_t)strtol(argv[COREDUMP_PID], &p, 10);
    if (*p != 0) {
        fprintf(stderr, "reading PID specifier failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    ret = snprintf(coredumpTargetFile, PATH_MAX, "%s/%s-%s-%s.coredump", coredumpConfig.path, applicationPath,
                   argv[COREDUMP_PID], argv[COREDUMP_TIME]);
    if (ret >= PATH_MAX) {
        fprintf(stderr, "coredump target filename to long, failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    fp = fopen(coredumpTargetFile, "w+");
    if (fp == NULL) {
        fprintf(stderr, "create coredump file '%s' failed!\n", coredumpTargetFile);
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    total = 0;
    while ((nread = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        size_t written = fwrite(buf, 1, nread, fp);
        if (written != nread) {
            fprintf(stderr, "write to coredump file '%s' failed!\n", coredumpTargetFile);
            fclose(fp);
            elosCoredumpConfigDelete(&coredumpConfig);
            return -1;
        }
        total += nread;
    }
    fprintf(stdout, "wrote coredump file '%s' with %zd bytes\n", coredumpTargetFile, total);

    fclose(fp);

    _replaceCharsInString(applicationPath, PATH_MAX, '_', '/');

    event.source.appName = applicationPath;
    event.source.fileName = applicationPath;

    event.severity = ELOS_SEVERITY_FATAL;
    event.hardwareid = argv[COREDUMP_HOSTNAME];
    event.classification = ELOS_CLASSIFICATION_PROCESS_ERRORS;
    event.messageCode = ELOS_MSG_CODE_CORE_DUMPED;

    ret = snprintf(payloadEvent, PATH_MAX + 128, "core dumped to %s, signal=%s, UID=%s, GID=%s", coredumpTargetFile,
                   argv[COREDUMP_SIGNAL], argv[COREDUMP_UID], argv[COREDUMP_GID]);
    if (ret >= (PATH_MAX + 128)) {
        fprintf(stderr, "coredump payload to long, failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    event.payload = payloadEvent;

    fprintf(stdout, "connecting coredump to event log scanner...\n");
    result = elosConnectTcpip("127.0.0.1", 54321, &session);
    if (result != SAFU_RESULT_OK) {
        fprintf(stderr, "coredump can not connect to elos and failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }

    fprintf(stdout, "send coredump event to log scanner...\n");
    result = elosEventPublish(session, &event);
    if (result != SAFU_RESULT_OK) {
        fprintf(stderr, "coredump push event failed!\n"); /* no return here, try to disc. */
    }

    fprintf(stdout, "disconnecting coredump from event log scanner...\n");
    result = elosDisconnect(session);
    if (result != SAFU_RESULT_OK) {
        fprintf(stderr, "coredump disconnect failed!\n");
        elosCoredumpConfigDelete(&coredumpConfig);
        return -1;
    }
    fprintf(stdout, "coredump done\n");

    elosCoredumpConfigDelete(&coredumpConfig);

    return ret;
}
