// SPDX-License-Identifier: MIT
#pragma once

#include <safu/log.h>
#include <safu/result.h>
#include <semaphore.h>

#define SAFU_SEM_WAIT_WITH_RESULT(__sem, __result) \
    if (sem_wait(__sem) != 0) {                    \
        (__result) = SAFU_RESULT_FAILED;           \
        safuLogErrErrno("sem_wait failed");        \
    } else {                                       \
        (__result) = SAFU_RESULT_OK;               \
    }

#define SAFU_SEM_POST_WITH_RESULT(__sem, __result) \
    if (sem_post(__sem) != 0) {                    \
        (__result) = SAFU_RESULT_FAILED;           \
        safuLogErrErrno("sem_post failed");        \
    } else {                                       \
        (__result) = SAFU_RESULT_OK;               \
    }

#define SAFU_SEM_WAIT_WITH_RESULT_IF_OK(__sem, __result) \
    if ((__result) == SAFU_RESULT_OK) {                  \
        if (sem_wait(__sem) != 0) {                      \
            (__result) = SAFU_RESULT_FAILED;             \
            safuLogErrErrno("sem_wait failed");          \
        } else {                                         \
            (__result) = SAFU_RESULT_OK;                 \
        }                                                \
    }

// More intuitive naming when using binary semaphores
#define SAFU_SEM_LOCK_WITH_RESULT       SAFU_SEM_WAIT_WITH_RESULT
#define SAFU_SEM_LOCK_WITH_RESULT_IF_OK SAFU_SEM_WAIT_WITH_RESULT_IF_OK
#define SAFU_SEM_UNLOCK_WITH_RESULT     SAFU_SEM_POST_WITH_RESULT
