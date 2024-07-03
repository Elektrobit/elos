// SPDX-License-Identifier: MIT

#pragma once

#include <safu/log.h>
#include <stdlib.h>

#include "elos/libelosplugin/libelosplugin.h"
#include "safu/common.h"

/*******************************************************************
 * Members:
 *     oomEventSubscriber   : the event subscriber structure
 *     oomEventPublisher    : the event publisher structure
 *     oomEventSubscription : the subscription structure
 ******************************************************************/
typedef struct elosOomKillerScanner {
    struct elosSubscriber *oomEventSubscriber;
    struct elosPublisher *oomEventPublisher;
    const elosSubscription_t *oomEventSubscription;
} elosOomKillerScanner_t;

/*************************************************************************
 * Allocate a new `elosOomKillerScanner`. It shall be safe to call
 * `elosOomKillerScannerStop` event when a `SAFU_RESULT_FAILED` is returned
 *
 * Parameters:
 *       oomKillerScanner : pointer to a `elosOomKillerScanner` pointer to
 *                          be set to the new OomKillerScanner. In case of
 *                          an error it is unchanged.
 *       plugin           : pointer to plugin context to which scanner members
 *                          are bound to
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ***************************************************************************/
safuResultE_t elosOomKillerScannerNew(elosOomKillerScanner_t **oomKillerScanner, elosPlugin_t *plugin);

/*******************************************************************
 * Start scanning ksmg log events for oom killer invocation. Publish
 * matched events.
 *
 * Parameters:
 *       oomKillerScanner : pointer to `elosOomKillerScanner` with subscriber
 *                          and subscription to match a oom killer invoked event
 *                          and a publisher to publish matched events
 *       plugin           : pointer to plugin context to which scanner members
 *                          are bound to
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ********************************************************************/
safuResultE_t elosOomKillerScannerStart(elosOomKillerScanner_t *oomKillerScanner, elosPlugin_t *plugin);

/*******************************************************************
 * Delete a `elosOomKillerScannerDelete`. It delete the `elosOomKillerScanner`
 *
 * Parameters:
 *       oomKillerScanner : pointer to a `elosOomKillerScanner_t` to be deleted.
 *       plugin           : pointer to plugin context to which scanner members
 *                          are bound to
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosOomKillerScannerDelete(elosOomKillerScanner_t *oomKillerScanner, elosPlugin_t *plugin);

/*******************************************************************
 * Stop the oomKillerScanner plugin to stop scanning ksmg events for
 * oom killer ivocation event
 *
 * Parameters:
 *       oomKillerScanner : pointer to `elosOomKillerScanner` with subscriber
 *                          and subscription to match a oom killer invoked event
 *                          and a publisher to publish matched events
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ********************************************************************/
safuResultE_t elosOomKillerScannerStop(elosOomKillerScanner_t *oomKillerScanner);
