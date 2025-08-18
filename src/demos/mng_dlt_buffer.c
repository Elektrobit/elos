// SPDX-License-Identifier: MIT

#include <errno.h>
#include <fcntl.h> /* For O_* constants */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <time.h>
#include <unistd.h>

#include "dlt_hv/types.h"

#define SHMEM_FILE_NAME "dlt_shmem_demo"

typedef struct {
    bool showUsage;
    bool dumpBuffer;
    bool unlinkBuffer;
    size_t bufferCount;
    char *shmemFile;
} elosConfig_t;

void _printUsage(FILE *stream, const char *const elosProgramName) {
    fprintf(stream,
            "Usage: %s [OPTION]...\n"
            "  -d         dump the buffer\n"
            "  -c <SIZE>  create a buffer with SIZE entries\n"
            "  -u         unlink the buffer\n"
            "  -f <FILE>  shared memory file name\n"
            "  -h         print this help\n",
            elosProgramName);
}

int elosInitConfig(int argc, char *argv[], elosConfig_t *config) {
    int res = 0;
    int c;

    config->showUsage = false;
    config->dumpBuffer = false;
    config->unlinkBuffer = false;
    config->shmemFile = NULL;
    config->bufferCount = 0;
    config->shmemFile = strdup(SHMEM_FILE_NAME);

    while (true) {
        c = getopt(argc, argv, "dc:uf:h");
        if (c == -1) {
            break;
        }
        switch (c) {
            case 'd':
                config->dumpBuffer = true;
                break;
            case 'c':
                config->bufferCount = strtoul(optarg, NULL, 10);
                break;
            case 'u':
                config->unlinkBuffer = true;
                break;
            case 'f':
                free(config->shmemFile);
                config->shmemFile = strdup(optarg);
                break;
            case 'h':
                config->showUsage = true;
                break;
            default:
                fprintf(stderr, "ERROR: Unrecognized option: '-%c'\n", optopt);
                config->showUsage = true;
                res = 1;
                break;
        }
    }
    return res;
}

void elosDeleteConfig(elosConfig_t *config) {
    free(config->shmemFile);
}

int _createShmemBuffer(size_t bufferSize, const char *shmemFileName) {
    int result = 0;
    size_t headerBytes = sizeof(elosEbLogRingBuffer_t);
    size_t bufferBytes = sizeof(elosEbLogEntry_t) * bufferSize;
    size_t shmemDataSize = headerBytes + bufferBytes;

    mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int oflag = O_RDWR | O_CREAT | O_TRUNC;
    int shmemFd = shm_open(shmemFileName, oflag, mode);
    if (shmemFd < 0) {
        fprintf(stderr, "failed to open shared memory!");
        result = 2;
    } else {
        int retVal = ftruncate(shmemFd, shmemDataSize);
        if (retVal != 0) {
            fprintf(stderr, "Error resizing shared memory!");
            result = 3;
        } else {
            void *shmp = mmap(NULL, shmemDataSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmemFd, 0);
            elosEbLogRingBuffer_t *buffer = shmp;
            buffer->entryCount = bufferSize;
            buffer->pad = 0xffff;
            buffer->idxRead = 1;
            buffer->idxWrite = bufferSize - 1;

            for (size_t i = 0; i < bufferSize; ++i) {
                struct timespec ts;
                timespec_get(&ts, TIME_UTC);
                elosEbLogEntry_t *entry = &buffer->entries[i];
                entry->creationTime = ts.tv_sec;
                entry->producerId = 0x16;
                entry->logLevel = 0x06;
                entry->pad[0] = 'P';
                entry->pad[1] = 'A';
                entry->pad[2] = 'D';
                entry->pad[3] = 'I';
                entry->pad[4] = 'N';
                entry->pad[5] = 'G';
                char creationTimeStr[100];
                strftime(creationTimeStr, sizeof creationTimeStr, "%d.%m.%Y %T", localtime(&ts.tv_sec));
#pragma GCC diagnostic ignored "-Wformat-truncation"
                snprintf(entry->logString, ELOS_EB_LOG_STRING_SIZE, "%c|0x%04lX log string text stuff %s|",
                         2 /*start of text*/, i, creationTimeStr);
#pragma GCC diagnostic pop
                entry->logString[ELOS_EB_LOG_STRING_SIZE - 1] = 3;  // end of text
            }
            munmap(shmp, shmemDataSize);
        }
    }
    close(shmemFd);
    return result;
}

static const char *elosLogLevelStr[] = {
    [ELOS_DLT_LOGLEVEL_OFF] = "ELOS_DLT_LOGLEVEL_OFF",     [ELOS_DLT_LOGLEVEL_FATAL] = "ELOS_DLT_LOGLEVEL_FATAL",
    [ELOS_DLT_LOGLEVEL_ERROR] = "ELOS_DLT_LOGLEVEL_ERROR", [ELOS_DLT_LOGLEVEL_WARN] = "ELOS_DLT_LOGLEVEL_WARN",
    [ELOS_DLT_LOGLEVEL_INFO] = "ELOS_DLT_LOGLEVEL_INFO",   [ELOS_DLT_LOGLEVEL_DEBUG] = "ELOS_DLT_LOGLEVEL_DEBUG",
};

static const char *elosMapLogLevel(elosDltLogLevelE_t log) {
    const char *res;
    switch (log) {
        case ELOS_DLT_LOGLEVEL_OFF:
        case ELOS_DLT_LOGLEVEL_FATAL:
        case ELOS_DLT_LOGLEVEL_ERROR:
        case ELOS_DLT_LOGLEVEL_WARN:
        case ELOS_DLT_LOGLEVEL_INFO:
        case ELOS_DLT_LOGLEVEL_DEBUG:
            res = elosLogLevelStr[log];
            break;
        default:
            res = "No Valid LogLevel";
    }
    return res;
}

static const char *elosUnicodeSub[] = {
    "⋄", "␁", "␂", "␃", "␄",  "␅", "␆", "␇", "␈", "␉", "␊", "␋", "␌",  "␍", "␎", "␏", "␐", "␑", "␒", "␓", "␔", "␕",
    "␖", "␗", "␘", "␙", "␚",  "␛", "␜", "␝", "␞", "␟", " ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+",
    ",", "-", ".", "/", "0",  "1", "2", "3", "4", "5", "6", "7", "8",  "9", ":", ";", "<", "=", ">", "?", "@", "A",
    "B", "C", "D", "E", "F",  "G", "H", "I", "J", "K", "L", "M", "N",  "O", "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d",  "e", "f", "g", "h", "i", "j", "k", "l", "m",
    "n", "o", "p", "q", "r",  "s", "t", "u", "v", "w", "x", "y", "z",  "{", "|", "}", "~", "␡", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x", "x",
    "x", "x", "x", "x", "x",  "x", "x", "x", "x", "x", "x", "x", "x",  "ﬀ",
};

void elosPrintShmemBuffer(const char *prefix, const uint8_t *buffer, size_t len) {
    printf("%s", prefix);
    for (size_t i = 0; i < len; ++i) {
        printf("%s", elosUnicodeSub[buffer[i]]);
    }
    printf("\n");
}
void elosPrintHexBuffer(const char *prefix, const uint8_t *buffer, size_t len) {
    printf("%s", prefix);
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
}

int _dumpShmemBuffer(const char *shmemFile) {
    int result = 0;
    int oflag = O_RDONLY;
    int shmemFd = shm_open(shmemFile, oflag, 0);
    if (shmemFd < 0) {
        fprintf(stderr, "failed to open shared memory file %s for reading\n", shmemFile);
        result = 4;
    } else {
        size_t headerBytes = sizeof(elosEbLogRingBuffer_t);
        void *shmp = mmap(NULL, headerBytes, PROT_READ, MAP_SHARED, shmemFd, 0);
        elosEbLogRingBuffer_t *buffer = shmp;
        size_t bufferSize = buffer->entryCount;
        size_t bufferBytes = sizeof(elosEbLogEntry_t) * bufferSize;
        printf("Header {\n");
        printf("   idxWrite = %u;\n", buffer->idxWrite);
        printf("   idxRead = %u;\n", buffer->idxRead);
        printf("   entryCount = %u;\n", buffer->entryCount);
        printf("   entries[] = ");
        size_t shmemDataSize = headerBytes + bufferBytes;
        shmp = mmap(NULL, shmemDataSize, PROT_READ, MAP_SHARED, shmemFd, 0);
        close(shmemFd);
        buffer = shmp;

        printf("{\n");
        for (size_t i = 0; i < bufferSize; ++i) {
            elosEbLogEntry_t *entry = &buffer->entries[i];
            char creationTimeStr[100];
            struct timespec ts = {.tv_sec = entry->creationTime};
            strftime(creationTimeStr, sizeof creationTimeStr, "%d.%m.%Y %T", localtime(&ts.tv_sec));
            printf("      [%lu] = Entry {\n", i);
            printf("         creationTime = %s;\n", creationTimeStr);
            printf("         producerId = %x;\n", entry->producerId);
            printf("         logLevel = %s;\n", elosMapLogLevel(entry->logLevel));
            printf("         logString = \"%.*s\"; (%lu/%u)\n", ELOS_EB_LOG_STRING_SIZE, entry->logString,
                   strlen(entry->logString), ELOS_EB_LOG_STRING_SIZE);
            elosPrintShmemBuffer("                   = ", (uint8_t *)entry->logString, ELOS_EB_LOG_STRING_SIZE / 2);
            elosPrintShmemBuffer("                     ", (uint8_t *)entry->logString + ELOS_EB_LOG_STRING_SIZE / 2,
                                 ELOS_EB_LOG_STRING_SIZE / 2);
            elosPrintHexBuffer("                   = ", (uint8_t *)entry->logString, ELOS_EB_LOG_STRING_SIZE / 4);
            elosPrintHexBuffer("                     ", (uint8_t *)entry->logString + ELOS_EB_LOG_STRING_SIZE / 4,
                               ELOS_EB_LOG_STRING_SIZE / 4);
            elosPrintHexBuffer("                     ", (uint8_t *)entry->logString + ELOS_EB_LOG_STRING_SIZE / 2,
                               ELOS_EB_LOG_STRING_SIZE / 4);
            elosPrintHexBuffer("                     ", (uint8_t *)entry->logString + 3 * ELOS_EB_LOG_STRING_SIZE / 4,
                               ELOS_EB_LOG_STRING_SIZE / 4);
            printf("      },\n");
        }
        printf("   }\n");
        printf("}\n");
        munmap(shmp, shmemDataSize);
    }
    return result;
}

int main(int argc, char *argv[]) {
    elosConfig_t config = {0};
    int res = elosInitConfig(argc, argv, &config);
    if (config.showUsage || res != 0) {
        _printUsage(res == 0 ? stdout : stderr, argv[0]);
    }
    if (config.bufferCount > 0 && res == 0) {
        res = _createShmemBuffer(config.bufferCount, config.shmemFile);
    }
    if (config.dumpBuffer && res == 0) {
        printf("# dump Config\n");
        res = _dumpShmemBuffer(config.shmemFile);
    }
    if (config.unlinkBuffer) {
        int unRes = shm_unlink(config.shmemFile);
        if (unRes != 0 && errno != ENOENT) {
            printf("failed to unlink shared memory file %s: %s\n", config.shmemFile, strerror(errno));
            res |= 8;
        }
    }
    elosDeleteConfig(&config);
    return res;
}
