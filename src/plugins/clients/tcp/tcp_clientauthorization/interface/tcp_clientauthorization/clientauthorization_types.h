// SPDX-License-Identifier: MIT
#pragma once

#include <libmnl/libmnl.h>
#include <netinet/in.h>
#include <safu/common.h>
#include <safu/vector.h>
#include <sys/types.h>

#define PRG_SOCKET_PFX          "socket:["
#define PRG_SOCKET_SUFF         "]"
#define PRG_SOCKET_PFXl         (strlen(PRG_SOCKET_PFX))
#define PRG_SOCKET_PFX2         "[0000]:"
#define PRG_SOCKET_PFX2l        (strlen(PRG_SOCKET_PFX2))
#define PATH_PROC               "/proc"
#define PATH_FD_SUFF            "fd"
#define PATH_FD_SUFFl           strlen(PATH_FD_SUFF)
#define PATH_PROC_X_FD          PATH_PROC "/%s/" PATH_FD_SUFF
#define ELOS_LINE_MAX           4096
#define PID_SIZE                10
#define PROC_NAME_LEN           256
#define PATH_EXELINE            "exe"
#define PATH_EXELINEl           strlen(PATH_EXELINE)
#define DECIMAL_DIGITS_BOUND(t) (241 * sizeof(t) / 100 + 2)
#define SOCKET_LINK_NAME_SIZE   (PRG_SOCKET_PFXl + DECIMAL_DIGITS_BOUND(ino_t) + 1)

typedef enum elosTcpStatusE {
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING
} elosTcpStatusE_t;

typedef struct elosClientAuthorization {
    struct mnl_socket *mlSocket;
    safuVec_t authorizedProcessFilters;
    int clientFd;
} elosClientAuthorization_t;
