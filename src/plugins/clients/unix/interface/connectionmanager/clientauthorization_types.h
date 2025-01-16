// SPDX-License-Identifier: MIT
#pragma once

#include <libmnl/libmnl.h>
#include <netinet/in.h>
#include <safu/common.h>
#include <safu/vector.h>
#include <sys/types.h>

typedef struct elosClientAuthorization {
    struct mnl_socket *mlSocket;
    safuVec_t authorizedProcessFilters;
} elosClientAuthorization_t;
