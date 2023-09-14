// SPDX-License-Identifier: MIT
#pragma once

#include <unistd.h>

typedef struct elosProcessIdentity {
    uid_t uid;
    gid_t gid;
    pid_t pid;
    char *exec;
} elosProcessIdentity_t;
