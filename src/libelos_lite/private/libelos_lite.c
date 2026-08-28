// SPDX-License-Identifier: MIT

#include "elos/libelos_lite.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

const char *elosliteGetLibraryVersion() {
    return ELOS_VERSION;
}

static bool _connectTcpipv4(const char *host, uint16_t port, elosliteSession_t *session);
static bool _connectTcpipv6(const char *host, uint16_t port, elosliteSession_t *session);

bool elosliteConnect(struct addrinfo addrInfo, elosliteSession_t *session) {
    int sfd = socket(addrInfo.ai_family, addrInfo.ai_socktype, addrInfo.ai_protocol);
    if (sfd == -1) {
        return false;
    }
    int retVal = connect(sfd, addrInfo.ai_addr, addrInfo.ai_addrlen);
    if (retVal == -1) {
        close(sfd);
        return false;
    }
    session->fd = sfd;
    session->connected = true;
    return true;
}

bool elosliteConnectTcpip(const char *host, uint16_t port, elosliteSession_t *session) {
    if (_connectTcpipv4(host, port, session)) {
        return true;
    }
    if (_connectTcpipv6(host, port, session)) {
        return true;
    }
    return false;
}

bool elosliteConnectUnix(const char *socketPath, elosliteSession_t *session) {
    if (socketPath == NULL) {
        return false;
    }
    if (session == NULL) {
        return false;
    }

    struct sockaddr_un address = {.sun_family = AF_UNIX, .sun_path = {0}};
    if (strlen(socketPath) + 1 > sizeof(address.sun_path)) {
        return false;
    }

    strncpy(address.sun_path, socketPath, strlen(socketPath));
    struct addrinfo addrinfo = {
        .ai_family = AF_UNIX,
        .ai_socktype = SOCK_STREAM,
        .ai_addr = (struct sockaddr *)&address,
        .ai_addrlen = sizeof(struct sockaddr_un),
    };

    return elosliteConnect(addrinfo, session);
}

bool _connectTcpipv4(const char *host, uint16_t port, elosliteSession_t *session) {
    struct in_addr inAddr = {0};
    if (!inet_aton(host, &inAddr)) {
        return false;
    }
    struct sockaddr_in sockaddrIn = {
        .sin_addr = inAddr,
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    struct addrinfo addinf = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
        .ai_addr = (struct sockaddr *)&sockaddrIn,
        .ai_addrlen = sizeof(sockaddrIn),
    };
    return elosliteConnect(addinf, session);
}

bool _connectTcpipv6(const char *host, uint16_t port, elosliteSession_t *session) {
    struct in6_addr inAddr = {0};
    if (!inet_pton(AF_INET6, host, &inAddr)) {
        return false;
    }
    struct sockaddr_in6 sockaddrIn = {
        .sin6_addr = inAddr,
        .sin6_family = AF_INET6,
        .sin6_port = htons(port),
    };
    struct addrinfo addinf = {
        .ai_family = AF_INET6,
        .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP,
        .ai_addr = (struct sockaddr *)&sockaddrIn,
        .ai_addrlen = sizeof(sockaddrIn),
    };
    return elosliteConnect(addinf, session);
}

bool elosliteDisconnect(elosliteSession_t *session) {
    if (session == NULL) {
        return false;
    }
    int retVal = close(session->fd);
    if (retVal < 0) {
        return false;
    }
    session->connected = false;

    return true;
}

bool elosliteFillEventSource(elosliteEventSource_t *source) {
    if (source == NULL) {
        return false;
    }

    char *appFilePath = (char *)getauxval(AT_EXECFN);
    if (appFilePath == NULL) {
        return false;
    }
    char *appName = &strrchr(appFilePath, '/')[1];

    source->appName = appName;
    source->fileName = appFilePath;
    source->pid = getpid();

    return true;
}

bool _readFileIntoBuffer(char *targetBuffer, const unsigned int bufSize, char *filename) {
    if (targetBuffer == NULL || filename == NULL) {
        return false;
    }

    const int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return false;
    }

    bool retval = false;
    struct stat fdStat;
    const int result = fstat(fd, &fdStat);
    if (result < 0) {
        retval = false;
        goto filereaderror;
    }

    size_t bytesTransferred = 0;
    size_t len = fdStat.st_size;
    const size_t initialLen = len;
    if (len > bufSize) {
        goto filereaderror;
    }

    while (bytesTransferred < initialLen) {
        int bytes;
        bytes = read(fd, &targetBuffer[bytesTransferred], len);
        if (bytes < 0) {
            switch (errno) {
                case EINTR:
                case EAGAIN:
                    continue;
                default:
                    break;
            }
        } else if (bytes == 0) {
            break;
        }
        len -= bytes;
        bytesTransferred += bytes;
    }
    if (bytesTransferred > 0) {
        retval = true;
    }

filereaderror:
    close(fd);
    return retval;
}

bool elosliteRetreiveMachineID(char **id) {
    static char machineID[ELOSD_HARDWAREID_MAX_LENGTH] = {0x00};
    static bool firstRun = true;

    if (id == NULL) {
        return false;
    }

    if (!firstRun) {
        *id = machineID;
        return true;
    }

    if (_readFileIntoBuffer(machineID, ELOSD_HARDWAREID_MAX_LENGTH, ELOSD_HARDWAREID_FILE) == false) {
        return false;
    }
    *id = machineID;
    firstRun = false;
    return true;
}

int _getMessageCode(elosSeverityE_t severity) {
    switch (severity) {
        case ELOS_SEVERITY_FATAL:
            return 1104;
        case ELOS_SEVERITY_ERROR:
            return 1105;
        case ELOS_SEVERITY_WARN:
            return 1106;
        case ELOS_SEVERITY_INFO:
            return 1102;
        case ELOS_SEVERITY_DEBUG:
            return 1101;
        case ELOS_SEVERITY_VERBOSE:
            return 1107;
        default:
            return 1000;
    }
}

bool elosliteLogMessageFormated(elosliteSession_t *session, elosSeverityE_t severity, char *fmtMessage, ...) {
    int ret = 0;
    elosliteEvent_t event = {0};
    char buffer[ELOSLITE_FMT_MAX_LOG_SIZE] = {'\0'};
    va_list args = {0};

    ret = clock_gettime(CLOCK_REALTIME, &event.date);
    if (ret < 0) {
        event.date.tv_sec = 0;
        event.date.tv_nsec = 0;
    }

    bool retval = elosliteFillEventSource(&event.source);
    if (retval == false) {
        return false;
    }
    retval = elosliteRetreiveMachineID(&event.hardwareid);
    if (retval == false) {
        return false;
    }

    event.severity = severity;
    event.classification = 0x0000000000000400ULL;  // ELOS_CLASSIFICATION_LOG
    event.messageCode = _getMessageCode(severity);

    va_start(args, fmtMessage);
    const size_t max_len = sizeof(buffer) - 1;
    const size_t len = (size_t)vsnprintf(buffer, max_len, fmtMessage, args);
    va_end(args);

    /* if 'len' exceeds 'max_len', truncate 'buffer' to 'max_len' and change last 3 char of 'buffer'
     * to ellipsis(...) to indicate truncation.
     */
    if (len >= max_len) {
        buffer[max_len - 3] = '.';
        buffer[max_len - 2] = '.';
        buffer[max_len - 1] = '.';
        buffer[max_len] = '\0';
    }

    event.payload = buffer;

    if (!eloslitePublish(session, &event)) {
        return false;
    }
    return true;
}
