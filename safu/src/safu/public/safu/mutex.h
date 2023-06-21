// SPDX-License-Identifier: MIT
#ifndef LIBELOS_MUTEX_H
#define LIBELOS_MUTEX_H

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include "log.h"

#define SAFU_PTHREAD_MUTEX_LOCK(__mutex, __fail_command)                  \
    if (pthread_mutex_lock(__mutex) < 0) {                                \
        safuLogCritF("pthread_mutex_lock failed! - %s", strerror(errno)); \
        raise(SIGTERM);                                                   \
        __fail_command;                                                   \
    }

#define SAFU_PTHREAD_MUTEX_UNLOCK(__mutex, __fail_command)                  \
    if (pthread_mutex_unlock(__mutex) < 0) {                                \
        safuLogCritF("pthread_mutex_unlock failed! - %s", strerror(errno)); \
        raise(SIGTERM);                                                     \
        __fail_command;                                                     \
    }

// We have to unlock the mutex here prior to destroying it to prevent undefined behavior according to POSIX
#define SAFU_PTHREAD_MUTEX_DESTROY(__mutex, __fail_command)                      \
    if (pthread_mutex_unlock(__mutex) < 0) {                                     \
        safuLogCritF("pthread_mutex_unlock failed! - %s", strerror(errno));      \
        raise(SIGTERM);                                                          \
        __fail_command;                                                          \
    } else {                                                                     \
        if (pthread_mutex_destroy(__mutex) < 0) {                                \
            ;                                                                    \
            safuLogCritF("pthread_mutex_destroy failed! - %s", strerror(errno)); \
            raise(SIGTERM);                                                      \
            __fail_command;                                                      \
        }                                                                        \
    }

#define SAFU_SEM_UNLOCK(__sem, __fail_command)                  \
    if (sem_post(__sem) < 0) {                                  \
        safuLogCritF("sem_post failed! - %s", strerror(errno)); \
        raise(SIGTERM);                                         \
        __fail_command;                                         \
    }

#define SAFU_PTHREAD_MUTEX_INIT_WITH_RESULT(__mutex, __mutexattr, __result) \
    if (pthread_mutex_init(__mutex, __mutexattr) < 0) {                     \
        (__result) = SAFU_RESULT_FAILED;                                    \
        safuLogCritF("pthread_mutex_init failed! - %s", strerror(errno));   \
        raise(SIGTERM);                                                     \
    } else {                                                                \
        (__result) = SAFU_RESULT_OK;                                        \
    }

#define SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT(__mutex, __result)            \
    if (pthread_mutex_lock(__mutex) < 0) {                                \
        (__result) = SAFU_RESULT_FAILED;                                  \
        safuLogCritF("pthread_mutex_lock failed! - %s", strerror(errno)); \
        raise(SIGTERM);                                                   \
    } else {                                                              \
        (__result) = SAFU_RESULT_OK;                                      \
    }

#define SAFU_PTHREAD_MUTEX_LOCK_WITH_RESULT_IF_OK(__mutex, __result)          \
    if ((__result) == SAFU_RESULT_OK) {                                       \
        if (pthread_mutex_lock(__mutex) < 0) {                                \
            (__result) = SAFU_RESULT_FAILED;                                  \
            safuLogCritF("pthread_mutex_lock failed! - %s", strerror(errno)); \
            raise(SIGTERM);                                                   \
        }                                                                     \
    }

#endif
