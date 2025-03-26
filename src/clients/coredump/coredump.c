// SPDX-License-Identifier: MIT

#define _GNU_SOURCE

#include <dirent.h>
#include <fcntl.h>
#include <ftw.h>
#include <limits.h>
#include <linux/limits.h>
#include <samconf/samconf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/libelos/libelos.h"
#include "safu/common.h"
#include "safu/log.h"

#define UNUSED __attribute__((unused))

#ifndef ELOS_COREDUMP_CONFIG_ROOT
#define ELOS_COREDUMP_CONFIG_ROOT "/root/elos/coredump/"
#endif

#define BUF_SIZE 1024

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
    unsigned long maxCoredumpMem;
    unsigned long maxCoredumpFileSize;
    int32_t coredumpFileCount;
    int32_t coredumpProcFileCount;
    char *networkAddress;
} elosCoredumpConfig_t;

static char elosOldestFile[PATH_MAX + 1] = {0};
struct timespec elosTimeModified = {0};
static unsigned long elosCoredumpSize = 0;
static int32_t elosCoredumpFileCount = 0;

static void _replaceCharsInString(char *str, uint32_t len, char old, char new) {
    uint32_t i = 0;

    while (str[i] != '\0' && i < len) {
        if (str[i] == old) str[i] = new;
        i++;
    }
}

static int _countFile(UNUSED const char *path, UNUSED const struct stat *sizeBuf, int typeflag,
                      UNUSED struct FTW *ftwbuf) {
    if (typeflag == FTW_F) {
        elosCoredumpFileCount += 1;
    }
    return 0;
}

static int _getCoredumpFileCount(const char *path) {
    int ret = 0;
    elosCoredumpFileCount = 0;

    ret = nftw(path, _countFile, 20, FTW_F);

    if (ret != 0) {
        fprintf(stderr, "cannot get file count in coredump dir failed\n");
    }

    return ret;
}

static int _calculateSize(UNUSED const char *path, UNUSED const struct stat *sizeBuf, int typeflag,
                          UNUSED struct FTW *ftwbuf) {
    if (typeflag == FTW_F) {
        elosCoredumpSize += sizeBuf->st_size;
    }
    return 0;
}

static int _getCoredumpFileSize(const char *path) {
    int ret = 0;
    elosCoredumpSize = 0;

    ret = nftw(path, _calculateSize, 20, FTW_F);

    if (ret != 0) {
        fprintf(stderr, "cannot get file size in coredump dir failed\n");
    }

    return ret;
}

static int _checkIfOlder(const char *path, const struct stat *sizeBuf, int typeflag, UNUSED struct FTW *ftwbuf) {
    if (typeflag == FTW_F) {
        if (elosTimeModified.tv_sec == 0 && elosTimeModified.tv_nsec == 0) {
            elosTimeModified.tv_sec = sizeBuf->st_mtim.tv_sec;
            elosTimeModified.tv_nsec = sizeBuf->st_mtim.tv_nsec;
            strcpy(elosOldestFile, path);
        } else if (sizeBuf->st_mtim.tv_sec < elosTimeModified.tv_sec) {
            elosTimeModified.tv_sec = sizeBuf->st_mtim.tv_sec;
            elosTimeModified.tv_nsec = sizeBuf->st_mtim.tv_nsec;
            strcpy(elosOldestFile, path);
        } else if (sizeBuf->st_mtim.tv_sec == elosTimeModified.tv_sec) {
            if (sizeBuf->st_mtim.tv_nsec < elosTimeModified.tv_nsec) {
                elosTimeModified.tv_sec = sizeBuf->st_mtim.tv_sec;
                elosTimeModified.tv_nsec = sizeBuf->st_mtim.tv_nsec;
                strcpy(elosOldestFile, path);
            }
        }
    }

    return 0;
}

static int _findOldestEntry(const char *path) {
    int ret = 0;

    ret = nftw(path, _checkIfOlder, 20, FTW_F);

    if (ret == 0) {
        if (elosOldestFile[0] == '\0') {
            fprintf(stdout, "no file found in path\n");
            ret = -1;
        }
    }

    return ret;
}

static int _deleteFile(const char *path) {
    int ret = 0;

    ret = unlink(path);
    if (ret != 0) {
        fprintf(stderr, "cannot delete file : %s \n", path);
    }

    return ret;
}

safuResultE_t elosCoredumpConfigInit(elosCoredumpConfig_t *coredumpConfig) {
    const char *location = NULL;
    samconfConfig_t *config = NULL;
    int32_t relMem = 0;
    int32_t absMem = 0;
    unsigned long fileSystemSize = 0;
    const char *pathname = NULL;
    struct statvfs dirBuf = {0};
    safuResultE_t result = SAFU_RESULT_OK;
    samconfConfigStatusE_t status = SAMCONF_CONFIG_ERROR;
    int ret = 0;

    location = safuGetEnvOr("ELOS_COREDUMP_CONFIG_FILE", ELOS_COREDUMP_CONFIG_FILE);

    fprintf(stdout, "loading coredump configuration at location %s\n", location);

    status = samconfLoad(location, false, &config);

    if (status != SAMCONF_CONFIG_OK) {
        fprintf(stderr, "loading coredump configuration failed \n");
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
                coredumpConfig->maxCoredumpMem = ((fileSystemSize * relMem) / 100);
            } else {
                fprintf(stderr, "coredump memory relative memory can not be zero or lesser");
                result = SAFU_RESULT_FAILED;
            }
        } else {
            status = samconfConfigGetInt32(config, ELOS_COREDUMP_CONFIG_ROOT "totalmem/", &absMem);
            if (status == SAMCONF_CONFIG_OK) {
                if (absMem > 0) {
                    coredumpConfig->maxCoredumpMem = absMem;
                } else {
                    fprintf(stderr, "coredump memory value can not be zero or lesser");
                    result = SAFU_RESULT_FAILED;
                }
            } else {
                fprintf(stderr, "neither totalmem nor totalmem_rel is set, memory calculation failed !\n");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        status = samconfConfigGetInt32(config, ELOS_COREDUMP_CONFIG_ROOT "maxcoredump_size_rel/", &relMem);
        if (status == SAMCONF_CONFIG_OK) {
            if (relMem >= 0) {
                coredumpConfig->maxCoredumpFileSize = ((fileSystemSize * relMem) / 100);
            } else {
                fprintf(stderr, "coredump per file relative memory value can not be zero or lesser");
                result = SAFU_RESULT_FAILED;
            }

        } else {
            status = samconfConfigGetInt32(config, ELOS_COREDUMP_CONFIG_ROOT "maxcoredump_size/", &absMem);
            if (status == SAMCONF_CONFIG_OK) {
                if (absMem >= 0) {
                    coredumpConfig->maxCoredumpFileSize = absMem;
                } else {
                    fprintf(stderr, "coredump per file memory value can not be zero or lesser");
                    result = SAFU_RESULT_FAILED;
                }
            }
        }
    }

    if (result == SAFU_RESULT_OK) {
        status = samconfConfigGetInt32(config, ELOS_COREDUMP_CONFIG_ROOT "coredump_per_exe/",
                                       &coredumpConfig->coredumpProcFileCount);
        if (status != SAMCONF_CONFIG_OK) {
            fprintf(stderr, "coredump count for each file can not be allocated");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        status = samconfConfigGetInt32(config, ELOS_COREDUMP_CONFIG_ROOT "maxcoredump_cnt/",
                                       &coredumpConfig->coredumpFileCount);
        if (status != SAMCONF_CONFIG_OK) {
            fprintf(stderr, "max coredump count can not be allocated");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        const char *address = NULL;
        status = samconfConfigGetString(config, ELOS_COREDUMP_CONFIG_ROOT "networkAddress/", &address);

        if (address != NULL && status == SAMCONF_CONFIG_OK) {
            coredumpConfig->networkAddress = strdup(address);
        } else {
            fprintf(stderr, "network address not found in config");
            result = SAFU_RESULT_FAILED;
        }
    }

    samconfConfigDelete(config);
    return result;
}

void elosCoredumpConfigDelete(elosCoredumpConfig_t *coredumpConfig) {
    free(coredumpConfig->networkAddress);
    free(coredumpConfig->path);
}

safuResultE_t elosCoredumpConnect(char *address, elosSession_t **session) {
    safuResultE_t result = SAFU_RESULT_OK;
    char interface[256];
    char portStr[6];

    if (address == NULL || *address == '\0') {
        fprintf(stderr, "Illegal network address format");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        int itemsScanned = sscanf(address, "%255[^:]:%5s", interface, portStr);

        if (itemsScanned == 2) {
            int port = strtol(portStr, NULL, 10);
            result = elosConnectTcpip(interface, port, session);
        } else {
            if (strchr(address, '/') != NULL) {
                result = elosConnectUnix(address, session);
            } else {
                fprintf(stderr, "Illegal network address format");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

safuResultE_t elosCoredumpPublishEvent(elosEvent_t *elosCoredumpEvent, char *address) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosSession_t *session;

    int ret = strcmp(elosCoredumpEvent->source.appName, "elosd");
    if (ret == 0) {
        fprintf(stdout, "Looks like elosd coredumped, do not send coredump event.\n");
    } else {
        fprintf(stdout, "connecting coredump to event log scanner...\n");

        result = elosCoredumpConnect(address, &session);
        if (result == SAFU_RESULT_OK) {
            fprintf(stdout, "send coredump event to log scanner...\n");
            result = elosEventPublish(session, elosCoredumpEvent);
            if (result == SAFU_RESULT_OK) {
                fprintf(stdout, "disconnecting coredump from event log scanner...\n");
                result = elosDisconnect(session);
            }
        }
    }

    return result;
}

safuResultE_t elosPrepareCorePattern(void) {
    char executablePath[PATH_MAX] = {0};
    const char *coredumpPattern = "%P %E %u %g %s %t %h";
    ssize_t count = 0;
    FILE *fp = NULL;
    int uid = 0;
    size_t nwrite = 0;
    safuResultE_t result = SAFU_RESULT_OK;

    uid = (int)geteuid();
    if (uid != 0) {
        fprintf(stderr, "prepare_coredump not called as root and failed!\n");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        count = readlink("/proc/self/exe", executablePath, PATH_MAX);
        if ((count == -1) || (count == PATH_MAX)) {
            fprintf(stderr, "prepare_coredump could not get its executable filepath and failed!\n");
            result = SAFU_RESULT_FAILED;
        }
    }

    if (result == SAFU_RESULT_OK) {
        fp = fopen("/proc/sys/kernel/core_pattern", "w");
        if (fp == NULL) {
            fprintf(stderr, "prepare_coredump could not open /proc/sys/kernel/core_pattern and failed!\n");
            result = SAFU_RESULT_FAILED;
        } else {
            nwrite = fprintf(fp, "|%s %s", executablePath, coredumpPattern);
            if (nwrite != (count + strlen(coredumpPattern) + 2)) {
                fprintf(stderr,
                        "prepare_coredump could not correctly write /proc/sys/kernel/core_pattern and failed!\n");
                result = SAFU_RESULT_FAILED;
            } else {
                fprintf(stdout, "coredump core_pattern setup done\n");
            }
            fclose(fp);
        }
    }

    return result;
}

static int _checkAndPrepareCoredump(elosCoredumpConfig_t *coredumpConfig, const char *appPath) {
    int32_t procFileCount = 0;
    int32_t totalFileCount = 0;
    int ret = 0;

    ret = _getCoredumpFileCount(coredumpConfig->path);
    if (ret == 0) {
        totalFileCount = elosCoredumpFileCount;

        ret = _getCoredumpFileCount(appPath);
        if (ret == 0) {
            procFileCount = elosCoredumpFileCount;

            ret = _getCoredumpFileSize(coredumpConfig->path);
            if (ret == 0) {
                if (procFileCount + 1 > coredumpConfig->coredumpProcFileCount ||
                    totalFileCount + 1 > coredumpConfig->coredumpFileCount ||
                    elosCoredumpSize + coredumpConfig->maxCoredumpFileSize > coredumpConfig->maxCoredumpMem) {
                    if (_findOldestEntry(appPath) == 0 || _findOldestEntry(coredumpConfig->path) == 0) {
                        if (_deleteFile(elosOldestFile) == 0) {
                            ret = 1;
                        } else {
                            ret = -1;
                        }
                    } else {
                        ret = -1;
                    }
                }
            }
        }
    }

    return ret;
}

static int _writeCoredump(const char *coredumpFile, unsigned long maxFileSize) {
    size_t nread = 0;
    size_t total = 0;
    FILE *fp = NULL;
    char buf[BUF_SIZE] = {0};
    int ret = 0;

    fp = fopen(coredumpFile, "w+");
    if (fp == NULL) {
        fprintf(stderr, "create coredump file '%s' failed!\n", coredumpFile);
        ret = -1;
    }

    if (ret == 0) {
        total = 0;
        while ((nread = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
            total += nread;
            if (total > maxFileSize) {
                fprintf(stderr, "Coredump size exceeds max coredump file size configured \n");
                ret = 1;
                break;
            }

            size_t written = fwrite(buf, 1, nread, fp);
            if (written != nread) {
                fprintf(stderr, "write to coredump file '%s' failed!\n", coredumpFile);
                ret = -1;
                break;
            }
        }

        if (ret == 1) {
            fprintf(stdout, "Discarding coredump file : %s \n", coredumpFile);
        } else if (ret == 0) {
            fprintf(stdout, "wrote coredump file '%s' with %zd bytes\n", coredumpFile, total);
        }

        fclose(fp);
    }

    return ret;
}

const char *_parseAppNameFromPath(const char *appPath) {
    const char *appName = strrchr(appPath, '/');

    if (appName != NULL && appName[1] != '\0') {
        appName = &appName[1];
    } else {
        appName = appPath;
    }

    return appName;
}

int main(int argc, char *argv[]) {
    char *tmpPtr = NULL;
    char coredumpTargetFile[PATH_MAX] = {0};
    char coredumpAppDir[PATH_MAX] = {0};
    char applicationPath[PATH_MAX] = {0};
    char payloadEvent[PATH_MAX + 128] = {0};
    int ret = 0;
    safuResultE_t result = SAFU_RESULT_OK;
    struct stat sizeBuf = {0};
    elosCoredumpConfig_t coredumpConfig = {0};
    elosEvent_t event = {0};

    fprintf(stdout, "coredump started\n");

    if (argc == 1) {
        fprintf(stdout, "coredump called without parameter - setup core_pattern\n");
        result = elosPrepareCorePattern();
        if (result != SAFU_RESULT_OK) {
            fprintf(stderr, "core dump pattern preparation failed\n");
            ret = -1;
        }
    } else if (argc != COREDUMP_MAX_SPECIFIER) {
        fprintf(stderr, "coredump called with invalid specifiers and failed!\n");
        ret = -1;
    } else {
        fprintf(stdout, "initializing coredump configuration\n");

        result = elosCoredumpConfigInit(&coredumpConfig);
        if (result != SAFU_RESULT_OK) {
            fprintf(stderr, "initializing coredump configuration failed \n");
            ret = -1;
        }

        if (ret == 0) {
            event.date.tv_sec = (time_t)strtoull(argv[COREDUMP_TIME], &tmpPtr, 10);
            if (*tmpPtr != 0) {
                fprintf(stderr, "reading timestamp specifier failed!\n");
                ret = -1;
            }
        }

        if (ret == 0) {
            event.source.pid = (pid_t)strtol(argv[COREDUMP_PID], &tmpPtr, 10);
            if (*tmpPtr != 0) {
                fprintf(stderr, "reading PID specifier failed!\n");
                ret = -1;
            }
        }

        if (ret == 0) {
            event.severity = ELOS_SEVERITY_FATAL;
            event.hardwareid = argv[COREDUMP_HOSTNAME];
            event.classification = ELOS_CLASSIFICATION_PROCESS_ERRORS;

            ret = snprintf(applicationPath, PATH_MAX, "%s", argv[COREDUMP_APP_PATH]);
            if (ret >= PATH_MAX) {
                fprintf(stderr, "reading application path specifier failed!\n");
            } else {
                ret = 0;
            }
        }

        if (ret == 0) {
            if (strchr(applicationPath, '!') != NULL) {
                _replaceCharsInString(applicationPath, PATH_MAX, '!', '_');
            } else if (strchr(applicationPath, '/') != NULL) {
                _replaceCharsInString(applicationPath, PATH_MAX, '/', '_');
            }

            ret = snprintf(coredumpAppDir, PATH_MAX, "%s/%s", coredumpConfig.path, applicationPath);

            if (ret >= PATH_MAX) {
                fprintf(stderr, "coredump application dir name too long, failed!\n");
            } else {
                ret = 0;
            }
        }

        if (ret == 0) {
            if (stat(coredumpAppDir, &sizeBuf) == -1) {
                if (mkdir(coredumpAppDir, S_IRWXU) == -1) {
                    fprintf(stderr, "Coredump dir for process does not exit and cannot be created, failed!\n");
                    ret = -1;
                }
            }
        }

        if (ret == 0) {
            ret = snprintf(coredumpTargetFile, PATH_MAX, "%s/%s/%s-%s.coredump", coredumpConfig.path, applicationPath,
                           argv[COREDUMP_TIME], argv[COREDUMP_PID]);
            if (ret >= PATH_MAX) {
                fprintf(stderr, "coredump target filename to long, failed!\n");
            } else {
                ret = 0;
            }
        }

        if (ret == 0) {
            _replaceCharsInString(applicationPath, PATH_MAX, '_', '/');
            event.source.appName = (char *)_parseAppNameFromPath(applicationPath);
            event.source.fileName = applicationPath;

            ret = _checkAndPrepareCoredump(&coredumpConfig, coredumpAppDir);
            if (ret == 1) {
                event.messageCode = ELOS_MSG_CODE_CORE_DUMP_DELETED;
                ret = snprintf(payloadEvent, PATH_MAX + 128, "core dump file %s deleted", elosOldestFile);
                if (ret < 0 || ret >= (PATH_MAX + 128)) {
                    fprintf(stderr, "payload string create failed will be set to %s\n", elosOldestFile);
                    event.payload = elosOldestFile;
                } else {
                    event.payload = payloadEvent;
                }
                result = elosCoredumpPublishEvent(&event, coredumpConfig.networkAddress);
                if (result != SAFU_RESULT_OK) {
                    fprintf(stderr, "coredump event publish failed\n");
                    ret = -1;
                } else {
                    ret = 0;
                }
            } else if (ret == -1) {
                fprintf(stderr, "coredump check returned error\n");
            }
        }

        if (ret == 0) {
            ret = _writeCoredump(coredumpTargetFile, coredumpConfig.maxCoredumpFileSize);
            if (ret == 1) {
                ret = _deleteFile(coredumpTargetFile);
                if (ret == 0) {
                    event.messageCode = ELOS_MSG_CODE_CORE_DUMP_DISCARDED;
                    ret = snprintf(payloadEvent, PATH_MAX + 128, "core dump file %s discarded", coredumpTargetFile);
                    if (ret < 0 || ret >= (PATH_MAX + 128)) {
                        fprintf(stderr, "payload string create failed will be set to %s\n", coredumpTargetFile);
                        event.payload = coredumpTargetFile;
                    } else {
                        event.payload = payloadEvent;
                    }
                    result = elosCoredumpPublishEvent(&event, coredumpConfig.networkAddress);
                    if (result != SAFU_RESULT_OK) {
                        fprintf(stderr, "coredump event publish failed\n");
                    }
                    ret = -1;
                }
            } else if (ret == -1) {
                fprintf(stderr, "writing to coredump file returned error\n");
            } else {
                event.messageCode = ELOS_MSG_CODE_CORE_DUMP_CREATED;
                ret = snprintf(payloadEvent, PATH_MAX + 128, "core dumped to %s, signal=%s, UID=%s, GID=%s",
                               coredumpTargetFile, argv[COREDUMP_SIGNAL], argv[COREDUMP_UID], argv[COREDUMP_GID]);
                if (ret < 0 || ret >= (PATH_MAX + 128)) {
                    fprintf(stderr, "payload string create failed will be set to %s\n", coredumpTargetFile);
                    event.payload = coredumpTargetFile;
                } else {
                    event.payload = payloadEvent;
                }
                result = elosCoredumpPublishEvent(&event, coredumpConfig.networkAddress);
                if (result != SAFU_RESULT_OK) {
                    fprintf(stderr, "coredump event publish failed\n");
                    ret = -1;
                } else {
                    fprintf(stdout, "coredump done\n");
                    ret = 0;
                }
            }
        }

        elosCoredumpConfigDelete(&coredumpConfig);
    }

    return (ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
