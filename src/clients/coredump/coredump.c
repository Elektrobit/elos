// SPDX-License-Identifier: MIT

#define _GNU_SOURCE
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/libelos/libelos.h"

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
    COREDUMP_PATHNAME,
    COREDUMP_UID,
    COREDUMP_GID,
    COREDUMP_SIGNAL,
    COREDUMP_TIME,
    COREDUMP_HOSTNAME,
    COREDUMP_MAX_SPECIFIER, /* must be the last entry */
};

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

int main(int argc, char *argv[]) {
    char *p;
    char buf[BUF_SIZE];
    char coredumpTargetFile[PATH_MAX];
    char pathnameProcess[PATH_MAX];
    char payloadEvent[PATH_MAX + 128];
    size_t nread;
    ssize_t total;
    FILE *fp;
    int ret;
    elosSession_t *session;
    elosEvent_t event = {0};

    fprintf(stdout, "coredump started\n");

    if (argc == 1) {
        fprintf(stdout, "coredump called without parameter - setup core_pattern\n");
        ret = elosPrepareCorePattern();
        return ret;
    } else if (argc != COREDUMP_MAX_SPECIFIER) {
        fprintf(stderr, "coredump called with invalid specifiers and failed!\n");
        return -1;
    }

    ret = snprintf(pathnameProcess, PATH_MAX, "%s", argv[COREDUMP_PATHNAME]);
    if (ret >= PATH_MAX) {
        fprintf(stderr, "reading pathname specifier failed!\n");
        return -1;
    }
    _replaceCharsInString(pathnameProcess, PATH_MAX, '!', '/');

    event.date.tv_sec = (time_t)strtoull(argv[COREDUMP_TIME], &p, 10);
    if (*p != 0) {
        fprintf(stderr, "reading timestamp specifier failed!\n");
        return -1;
    }

    event.source.pid = (pid_t)strtol(argv[COREDUMP_PID], &p, 10);
    if (*p != 0) {
        fprintf(stderr, "reading PID specifier failed!\n");
        return -1;
    }

    ret = snprintf(coredumpTargetFile, PATH_MAX, "%s/core.%s.%s", COREDUMP_TARGET_PATH, argv[COREDUMP_TIME],
                   argv[COREDUMP_PID]);
    if (ret >= PATH_MAX) {
        fprintf(stderr, "coredump target filename to long, failed!\n");
        return -1;
    }

    fp = fopen(coredumpTargetFile, "w+");
    if (fp == NULL) {
        fprintf(stderr, "create coredump file '%s' failed!\n", coredumpTargetFile);
        return -1;
    }

    total = 0;
    while ((nread = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        size_t written = fwrite(buf, 1, nread, fp);
        if (written != nread) {
            fprintf(stderr, "write to coredump file '%s' failed!\n", coredumpTargetFile);
            fclose(fp);
            return -1;
        }
        total += nread;
    }
    fprintf(stdout, "wrote coredump file '%s' with %zd bytes\n", coredumpTargetFile, total);

    fclose(fp);

    event.source.appName = pathnameProcess;
    event.source.fileName = pathnameProcess;

    event.severity = ELOS_SEVERITY_FATAL;
    event.hardwareid = argv[COREDUMP_HOSTNAME];
    event.classification = ELOS_CLASSIFICATION_PROCESS_ERRORS;
    event.messageCode = ELOS_MSG_CODE_CORE_DUMPED;

    ret = snprintf(payloadEvent, PATH_MAX + 128, "core dumped to %s, signal=%s, UID=%s, GID=%s", coredumpTargetFile,
                   argv[COREDUMP_SIGNAL], argv[COREDUMP_UID], argv[COREDUMP_GID]);
    if (ret >= (PATH_MAX + 128)) {
        fprintf(stderr, "coredump payload to long, failed!\n");
        return -1;
    }

    event.payload = payloadEvent;

    fprintf(stdout, "connecting coredump to event log scanner...\n");
    ret = elosConnectTcpip("127.0.0.1", 54321, &session);
    if (ret < 0) {
        fprintf(stderr, "coredump can not connect to elos and failed!\n");
        return -1;
    }

    fprintf(stdout, "send coredump event to log scanner...\n");
    ret = elosEventPublish(session, &event);
    if (ret) fprintf(stderr, "coredump push event failed!\n"); /* no return here, try to disc. */

    fprintf(stdout, "disconnecting coredump from event log scanner...\n");
    ret = elosDisconnect(session);
    if (ret) {
        fprintf(stderr, "coredump disconnect failed!\n");
        return -1;
    }
    fprintf(stdout, "coredump done\n");

    return ret;
}
