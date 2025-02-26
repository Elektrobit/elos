// SPDX-License-Identifier: MIT
#include "tcp_clientauthorization/clientauthorization.h"

#include <arpa/inet.h>
#include <asm/types.h>
#include <bits/stdint-uintn.h>
#include <ctype.h>
#include <dirent.h>
#include <elos/common/process.h>
#include <elos/libelosplugin/clientauthorizedprocesses.h>
#include <errno.h>
#include <fcntl.h>
#include <libmnl/libmnl.h>
#include <limits.h>
#include <linux/inet_diag.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/sock_diag.h>
#include <linux/tcp.h>
#include <netinet/in.h>
#include <pwd.h>
#include <safu/common.h>
#include <safu/log.h>
#include <safu/vector.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define LIBMNL_OK    0
#define LIBMNL_ERROR 1

static int _extractType1SocketInode(const char *linkName, unsigned long *inodePtr) {
    size_t linkNameLength = strlen(linkName);
    char inodeStr[linkNameLength - 1];
    memset(inodeStr, 0, sizeof(inodeStr));
    const int inodeStrLen = linkNameLength - PRG_SOCKET_PFXl - 1;
    char *serr = NULL;
    int retval = -1;

    if (linkNameLength >= PRG_SOCKET_PFXl + 3) {
        if (memcmp(linkName, PRG_SOCKET_PFX, PRG_SOCKET_PFXl) == 0) {
            if (linkName[linkNameLength - 1] == ']') {
                strncpy(inodeStr, linkName + PRG_SOCKET_PFXl, inodeStrLen);
                inodeStr[inodeStrLen] = '\0';
                *inodePtr = strtoul(inodeStr, &serr, 0);

                if (serr != NULL && serr[0] == '\0' && *inodePtr != ULONG_MAX) {
                    retval = 0;
                }
            }
        }
    }

    return retval;
}

static int _extractType2SocketInode(const char *linkName, unsigned long *inodePtr) {
    size_t linkNameLength = strlen(linkName);
    int retval = -1;

    if (linkNameLength > PRG_SOCKET_PFX2l + 1) {
        if (memcmp(linkName, PRG_SOCKET_PFX2, PRG_SOCKET_PFX2l) == 0) {
            char *serr;
            *inodePtr = strtoul(linkName + PRG_SOCKET_PFX2l, &serr, 0);

            if (serr != NULL && serr[0] == '\0' && *inodePtr != ULONG_MAX) {
                retval = 0;
            }
        }
    }

    return retval;
}

static int _isPidDirectory(const char *dirName) {
    const char *procDirName;
    int retval = 0;
    for (procDirName = dirName; *procDirName; procDirName++) {
        if (!isdigit(*procDirName)) {
            break;
        }
    }
    if (*procDirName) {
        retval = 1;
    }

    return retval;
}

static inline safuResultE_t _getExecPath(char const *const line, char **execPath) {
    safuResultE_t result = SAFU_RESULT_OK;
    char prog[PATH_MAX] = {0};

    ssize_t length = readlink(line, prog, PATH_MAX - 1);
    if (length == -1) {
        result = SAFU_RESULT_FAILED;
    } else {
        prog[length] = '\0';
        *execPath = strdup(prog);
        if (*execPath == NULL) {
            safuLogErr("Failed to allocate string");
            result = SAFU_RESULT_FAILED;
        }
    }

    return result;
}

static inline safuResultE_t _getPid(struct dirent const *const direproc, pid_t *const socketPid) {
    safuResultE_t result = SAFU_RESULT_OK;
    char *endPtr = NULL;

    *socketPid = strtoul(direproc->d_name, &endPtr, 10);
    if (direproc->d_name[0] != '\0' && endPtr[0] != '\0') {
        *socketPid = -1;
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

static safuResultE_t _getSocketPid(unsigned long socketInode, pid_t *socketPid, char **execPath) {
    safuResultE_t result = SAFU_RESULT_OK;
    char line[ELOS_LINE_MAX] = {0};
    bool eacces = false;
    int retval = 0;
    ssize_t procfdlen, linkNameLength;
    char linkName[SOCKET_LINK_NAME_SIZE];
    unsigned long inode;
    DIR *dirproc = NULL, *dirfd = NULL;
    struct dirent *direproc, *direfd;

    dirproc = opendir(PATH_PROC);
    if (dirproc != NULL) {
        while (errno = 0, direproc = readdir(dirproc)) {
            retval = _isPidDirectory(direproc->d_name);
            if (retval != 0) {
                continue;
            }

            procfdlen = snprintf(line, sizeof(line), PATH_PROC_X_FD, direproc->d_name);
            if (procfdlen <= 0 || procfdlen >= (int)(sizeof(line) - 5)) {
                continue;
            }

            errno = 0;
            dirfd = opendir(line);
            if (dirfd == NULL) {
                if (errno == EACCES) {
                    eacces = true;
                }
                continue;
            }
            line[procfdlen] = '/';
            while ((direfd = readdir(dirfd))) {
                if (!isdigit(direfd->d_name[0])) {
                    continue;
                }
                if (procfdlen + 1 + strlen(direfd->d_name) + 1 > sizeof(line)) {
                    continue;
                }
                memcpy(line + procfdlen - PATH_FD_SUFFl, PATH_FD_SUFF "/", PATH_FD_SUFFl + 1);
                strncpy(line + procfdlen + 1, direfd->d_name, sizeof(line) - procfdlen - 1);
                linkNameLength = readlink(line, linkName, sizeof(linkName) - 1);
                if (linkNameLength == -1) {
                    continue;
                }
                linkName[linkNameLength] = '\0';

                if (_extractType1SocketInode(linkName, &inode) < 0) {
                    if (_extractType2SocketInode(linkName, &inode) < 0) {
                        continue;
                    }
                }

                if (inode == socketInode) {
                    if (procfdlen - PATH_FD_SUFFl + PATH_EXELINEl >= sizeof(line) - 5) {
                        continue;
                    }

                    strncpy(line + procfdlen - PATH_FD_SUFFl, PATH_EXELINE, sizeof(line) - procfdlen + PATH_FD_SUFFl);
                    result = _getExecPath(line, execPath);
                    result = _getPid(direproc, socketPid);
                }
            }
            closedir(dirfd);
            dirfd = NULL;
            if (result != SAFU_RESULT_OK) {
                break;
            }
        }
    } else {
        safuLogErrErrno("Application should be run as root\n");
        result = SAFU_RESULT_FAILED;
    }

    if (dirproc) {
        closedir(dirproc);
    }
    if (eacces) {
        result = SAFU_RESULT_FAILED;
    }

    return result;
}

static unsigned char _createSocketFilter(void **filterMem, u_int16_t sockport) {
    unsigned char filterLength = sizeof(struct inet_diag_bc_op) * 2;
    struct inet_diag_bc_op *bytecodeOperation = NULL;

    bytecodeOperation = calloc(filterLength, 1);
    if (bytecodeOperation == NULL) {
        filterLength = 0;
    } else {
        bytecodeOperation[0].code = INET_DIAG_BC_S_EQ;
        bytecodeOperation[0].yes = filterLength;
        bytecodeOperation[0].no = 12;

        bytecodeOperation[1].no = sockport;

        *filterMem = bytecodeOperation;
    }

    return filterLength;
}

static int _requestSocketInfo(struct mnl_socket *netlinkSock, u_int16_t socketPort) {
    int retval = 0;
    uint8_t msgBuf[MNL_SOCKET_BUFFER_SIZE];
    struct nlmsghdr *nlh = NULL;
    struct inet_diag_req_v2 *connectionRequest = NULL;

    memset(msgBuf, 0, sizeof(msgBuf));

    nlh = mnl_nlmsg_put_header(msgBuf);
    nlh->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
    nlh->nlmsg_type = SOCK_DIAG_BY_FAMILY;
    nlh->nlmsg_pid = mnl_socket_get_portid(netlinkSock);

    connectionRequest = mnl_nlmsg_put_extra_header(nlh, sizeof(struct inet_diag_req_v2));
    connectionRequest->sdiag_family = AF_INET;
    connectionRequest->sdiag_protocol = IPPROTO_TCP;
    connectionRequest->idiag_states = 1 << TCP_ESTABLISHED;
    connectionRequest->idiag_ext |= (1 << (INET_DIAG_INFO - 1));

    void *filterMem = NULL;
    int filterLength = _createSocketFilter(&filterMem, socketPort);

    if (filterLength > 0) {
        mnl_attr_put(nlh, INET_DIAG_REQ_BYTECODE, filterLength, filterMem);
        retval = mnl_socket_sendto(netlinkSock, msgBuf, nlh->nlmsg_len);
    } else {
        retval = LIBMNL_ERROR;
    }

    free(filterMem);

    return retval;
}

static safuResultE_t _lookupProcess(struct inet_diag_msg *diagMsg, elosProcessIdentity_t *const process) {
    safuResultE_t result = SAFU_RESULT_OK;
    char localAddresBuffer[INET6_ADDRSTRLEN] = {0};
    char remoteAddressBuffer[INET6_ADDRSTRLEN] = {0};

    if (diagMsg->idiag_family == AF_INET) {
        inet_ntop(AF_INET, (struct in_addr *)&(diagMsg->id.idiag_src), localAddresBuffer, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, (struct in_addr *)&(diagMsg->id.idiag_dst), remoteAddressBuffer, INET_ADDRSTRLEN);
    } else if (diagMsg->idiag_family == AF_INET6) {
        inet_ntop(AF_INET6, (struct in_addr6 *)&(diagMsg->id.idiag_src), localAddresBuffer, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, (struct in_addr6 *)&(diagMsg->id.idiag_dst), remoteAddressBuffer, INET6_ADDRSTRLEN);
    } else {
        safuLogErrErrno("Unknown family\n");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        if (localAddresBuffer[0] != 0 && remoteAddressBuffer[0] != 0) {
            safuLogDebug("Start process look up");
            process->uid = diagMsg->idiag_uid;
            _getSocketPid(diagMsg->idiag_inode, &process->pid, &process->exec);

            struct passwd *uidInfo = getpwuid(diagMsg->idiag_uid);
            process->gid = uidInfo != NULL ? uidInfo->pw_gid : 0;

            safuLogDebugF(
                "==Socket Info== \n Client : %s \n User: %s (PID: %u UID: %u GID: %u) Src: %s:%d Dst: %s:%d\n",
                process->exec, uidInfo == NULL ? "Not found" : uidInfo->pw_name, process->pid, process->uid,
                process->gid, localAddresBuffer, ntohs(diagMsg->id.idiag_sport), remoteAddressBuffer,
                ntohs(diagMsg->id.idiag_dport));
        } else {
            safuLogErrErrno("Could not get required connection information\n");
            result = SAFU_RESULT_FAILED;
        }
    }
    return result;
}

static safuResultE_t _processBuffer(uint8_t const *const recvBuffer, ssize_t size, safuVec_t *processes,
                                    bool *isStreamDone) {
    safuResultE_t result = SAFU_RESULT_OK;
    struct nlmsghdr *nlh = NULL;
    struct nlmsgerr *nerr = NULL;
    int numbytes = size;

    nlh = (struct nlmsghdr *)recvBuffer;
    while (mnl_nlmsg_ok(nlh, numbytes)) {
        safuLogDebugF("got netlink message (%d)", nlh->nlmsg_type);

        if (nlh->nlmsg_type == NLMSG_DONE) {
            result = SAFU_RESULT_OK;
            *isStreamDone = true;
            break;
        } else if (nlh->nlmsg_type == NLMSG_ERROR) {
            *isStreamDone = true;
            nerr = mnl_nlmsg_get_payload(nlh);
            if (nerr->error) {
                safuLogErr("Error in netlink error message ?");
                result = SAFU_RESULT_FAILED;
                break;
            } else {
                safuLogErr("Received netlink error message");
            }
        } else {
            struct inet_diag_msg *diagMsg = mnl_nlmsg_get_payload(nlh);

            elosProcessIdentity_t process = {0};
            result = _lookupProcess(diagMsg, &process);
            if (result == SAFU_RESULT_OK) {
                safuVecPush(processes, &process);
            }
        }

        nlh = mnl_nlmsg_next(nlh, &numbytes);
    }
    return result;
}

safuResultE_t elosTcpClientAuthorizationInitialize(elosClientAuthorization_t *const clientAuth) {
    safuResultE_t result = SAFU_RESULT_OK;
    struct mnl_socket *mlSock = NULL;

    if (clientAuth == NULL) {
        safuLogErr("Invalid argument");
        result = SAFU_RESULT_FAILED;
    } else {
        safuLogDebug("open netlink socket");

        mlSock = mnl_socket_open(NETLINK_INET_DIAG);
        if (mlSock == NULL) {
            safuLogErrErrno("Netlink socket connection creation failed");
            result = SAFU_RESULT_FAILED;
        } else {
            int ret = mnl_socket_bind(mlSock, 0, MNL_SOCKET_AUTOPID);
            if (ret == LIBMNL_OK) {
                clientAuth->socketData = (void *)mlSock;
            } else {
                safuLogErrErrno("Netlink bind socket failed");
                result = SAFU_RESULT_FAILED;
            }
        }
    }

    return result;
}

bool elosTcpClientAuthorizationIsTrustedConnection(elosClientAuthorization_t *const clientAuth,
                                                   struct sockaddr const *const addr) {
    int numbytes = 0;
    uint8_t recvBuffer[MNL_SOCKET_BUFFER_SIZE];
    struct mnl_socket *mlSock = (struct mnl_socket *)clientAuth->socketData;
    safuResultE_t result = SAFU_RESULT_OK;
    bool isClientAuthorized = false;
    struct sockaddr_in *addrIp = (struct sockaddr_in *)addr;

    safuLogDebug("Check for trusted connection");

    if (_requestSocketInfo(mlSock, ntohs(addrIp->sin_port)) < 0) {
        safuLogErrErrno("Failed to retrieve socket information");
        result = SAFU_RESULT_FAILED;
    }

    safuVec_t processes;
    int retval = safuVecCreate(&processes, 2, sizeof(elosProcessIdentity_t));
    if (retval == -1) {
        safuLogErr("Failed to allocate memory");
        result = SAFU_RESULT_FAILED;
    }

    if (result == SAFU_RESULT_OK) {
        bool isStreamDone = false;
        while (isStreamDone == false) {
            numbytes = mnl_socket_recvfrom(mlSock, recvBuffer, sizeof(recvBuffer));
            if (numbytes == -1) {
                safuLogErrErrno("Failed to receive netlink message");
                result = SAFU_RESULT_FAILED;
                isStreamDone = true;
            } else {
                result = _processBuffer(recvBuffer, numbytes, &processes, &isStreamDone);
            }
        }

        size_t processCount = safuVecElements(&processes);

        for (size_t i = 0; i < processCount; i++) {
            elosProcessIdentity_t *process = safuVecGet(&processes, i);
            if (isClientAuthorized == false) {
                isClientAuthorized = elosAuthorizedProcessCheck(&clientAuth->authorizedProcessFilters, process);
            }
            free(process->exec);
        }
        safuVecFree(&processes);
    }

    return isClientAuthorized;
}

safuResultE_t elosTcpClientAuthorizationDelete(elosClientAuthorization_t *const clientAuth) {
    safuResultE_t result = SAFU_RESULT_OK;

    if (clientAuth == NULL) {
        safuLogErr("Invalid argument");
        result = SAFU_RESULT_FAILED;
    } else {
        safuLogDebug("close netlink socket");
        struct mnl_socket *mlSock = (struct mnl_socket *)clientAuth->socketData;
        if (mlSock != NULL) {
            int ret = mnl_socket_close(mlSock);
            if (ret != LIBMNL_OK) {
                result = SAFU_RESULT_FAILED;
            } else {
                clientAuth->socketData = NULL;
            }
        }

        if (clientAuth->authorizedProcessFilters.elementSize > 0) {
            result = elosAuthorizedProcessDelete(&clientAuth->authorizedProcessFilters);
        }
    }

    return result;
}
