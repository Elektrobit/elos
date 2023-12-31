// SPDX-License-Identifier: MIT
#pragma once

#include <safu/flags.h>

#define ELOS_PLUGINCONTROL_FLAG_LOADED_BIT      (SAFU_FLAG_CUSTOM_START_BIT << 0)
#define ELOS_PLUGINCONTROL_FLAG_STARTED_BIT     (SAFU_FLAG_CUSTOM_START_BIT << 1)
#define ELOS_PLUGINCONTROL_FLAG_WORKER_BIT      (SAFU_FLAG_CUSTOM_START_BIT << 2)
#define ELOS_PLUGINCONTROL_FLAG_PLUGINERROR_BIT (SAFU_FLAG_CUSTOM_START_BIT << 3)

#define ELOS_PLUGINCONTROL_FLAG_HAS_LOADED_BIT(__f)  ((atomic_load(__f) & ELOS_PLUGINCONTROL_FLAG_LOADED_BIT) != 0)
#define ELOS_PLUGINCONTROL_FLAG_HAS_STARTED_BIT(__f) ((atomic_load(__f) & ELOS_PLUGINCONTROL_FLAG_STARTED_BIT) != 0)
#define ELOS_PLUGINCONTROL_FLAG_HAS_WORKER_BIT(__f)  ((atomic_load(__f) & ELOS_PLUGINCONTROL_FLAG_WORKER_BIT) != 0)
#define ELOS_PLUGINCONTROL_FLAG_HAS_PLUGINERROR_BIT(__f) \
    ((atomic_load(__f) & ELOS_PLUGINCONTROL_FLAG_PLUGINERROR_BIT) != 0)

/*******************************************************************
 * Atomic set bit and test bit.
 * Returns 'true' when the given bit went from 0 to 1 during the operation.
 *
 * Parameters:
 *      __f : Pointer to a safuFlags_t field
 *      __b : The bit to be set and tested.
 *
 * Returns:
 *      - `true` when the given bit went from 0 to 1 during the operation.
 *      - `false` when the given bit did not change
 ******************************************************************/
#define ELOS_PLUGINCONTROL_FLAG_PUSH_BIT(__f, __b) ((atomic_fetch_or(__f, (__b)) & (__b)) == 0)

/*******************************************************************
 * Atomic clear bit and test bit.
 * Is 'true' when the given bit went from 1 to 0 during the operation
 *
 * Parameters:
 *      __f : Pointer to a safuFlags_t field
 *      __b : The bit to be cleared and tested.
 *
 * Returns:
 *      - `true` when the given bit went from 1 to 0 during the operation
 *      - `false` when the given bit did not change
 ******************************************************************/
#define ELOS_PLUGINCONTROL_FLAG_PULL_BIT(__f, __b) ((atomic_fetch_and(__f, ~(__b)) & (__b)) != 0)

#define ELOS_PLUGINCONTROL_FLAG_PUSH_LOADED_BIT(__f) \
    ELOS_PLUGINCONTROL_FLAG_PUSH_BIT(__f, ELOS_PLUGINCONTROL_FLAG_LOADED_BIT)

#define ELOS_PLUGINCONTROL_FLAG_PUSH_STARTED_BIT(__f) \
    ELOS_PLUGINCONTROL_FLAG_PUSH_BIT(__f, ELOS_PLUGINCONTROL_FLAG_STARTED_BIT)

#define ELOS_PLUGINCONTROL_FLAG_PUSH_WORKER_BIT(__f) \
    ELOS_PLUGINCONTROL_FLAG_PUSH_BIT(__f, ELOS_PLUGINCONTROL_FLAG_WORKER_BIT)

#define ELOS_PLUGINCONTROL_FLAG_PULL_LOADED_BIT(__f) \
    ELOS_PLUGINCONTROL_FLAG_PULL_BIT(__f, ELOS_PLUGINCONTROL_FLAG_LOADED_BIT)

#define ELOS_PLUGINCONTROL_FLAG_PULL_STARTED_BIT(__f) \
    ELOS_PLUGINCONTROL_FLAG_PULL_BIT(__f, ELOS_PLUGINCONTROL_FLAG_STARTED_BIT)

#define ELOS_PLUGINCONTROL_FLAG_PULL_WORKER_BIT(__f) \
    ELOS_PLUGINCONTROL_FLAG_PULL_BIT(__f, ELOS_PLUGINCONTROL_FLAG_WORKER_BIT)
