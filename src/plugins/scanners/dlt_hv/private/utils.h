// SPDX-License-Identifier: MIT

#pragma once

#include <elos/event/event.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/result.h>

#include "dlt_hv/types.h"

/*************************************************
 * transform a `elosEbLogEntry_t` into an `elosEvent_t`
 *
 * Parameters:
 *   entry: the dlt log entry to transform
 *   event: the elos event to write the parsed log entry to
 *          filds not set by the log entry arn't touch
 *          and stay as they are
 *
 * Returns:
 *   - SAFU_RESULT_OK: on success
 *   - SAFU_RESULT_FAILED: when entry or event arn't set propperly
 *************************************************/
safuResultE_t elosEventFromLogEntry(elosEbLogEntry_t *entry, elosEvent_t *event);

/*************************************************
 * publish an `elosEvent_t` and also store it to the storage backends
 *
 * Parameters:
 *   plugin: the referecne to the plugin
 *   publisher: the publisher to use
 *   event: the event to publish and store
 *
 * Returns:
 *   - SAFU_RESULT_OK: on success
 *   - otherwise the error result of `elosEventPublish` or `elosEventStore`
 *************************************************/
safuResultE_t elosPublishAndStore(elosPlugin_t *plugin, struct elosPublisher *publisher, elosEvent_t *event);
