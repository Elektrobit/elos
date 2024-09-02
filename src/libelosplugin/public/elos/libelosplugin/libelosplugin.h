// SPDX-License-Identifier: MIT
#pragma once

#include <safu/vector.h>

#include "elos/libelosplugin/types.h"

__BEGIN_DECLS

safuResultE_t elosPluginReportAsStarted(elosPlugin_t *plugin);
safuResultE_t elosPluginStopTriggerWait(elosPlugin_t *plugin);
safuResultE_t elosPluginStopTriggerWrite(elosPlugin_t *plugin);
const char *elosPluginTypeToStr(elosPluginTypeE_t type);

/******************************************************************************
 * A new publisher will be allocated and stored in the provided pointer. On any
 * failure the publisher pointer remains unchanged.
 *
 * Parameters:
 *      plugin    : Pointer of a plugin data structure
 *      publisher : Pointer to a pointer where to store the pointer of the new
 *                  publisher in case of success, otherwise it remains unchanged
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
inline safuResultE_t elosPluginCreatePublisher(elosPlugin_t *const plugin, struct elosPublisher **publisher) {
    return plugin->createPublisher(plugin->instanceRef, publisher);
}

/******************************************************************************
 * Deletes a publisher and frees all related resources. On error it is unsafe
 * to retry or use the publisher.
 *
 * Parameters:
 *      plugin    : Pointer of a plugin data structure
 *      publisher : Pointer to a publisher to be deleted.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
inline safuResultE_t elosPluginDeletePublisher(elosPlugin_t *const plugin, struct elosPublisher *publisher) {
    return plugin->deletePublisher(plugin->instanceRef, publisher);
}

/******************************************************************************
 *
 * A new subscriber will be allocated and stored in the provided pointer. On any
 * failure the subscriber pointer remains unchanged.
 *
 * Parameters:
 *      plugin     : Pointer of a plugin data structure
 *      subscriber : Pointer to a pointer where to store the pointer of the new
 *                   subscriber in case of success, otherwise it remains
 *                   unchanged
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
inline safuResultE_t elosPluginCreateSubscriber(elosPlugin_t *const plugin, struct elosSubscriber **subscriber) {
    return plugin->createSubscriber(plugin->instanceRef, subscriber);
}

/******************************************************************************
 * Deletes a publisher and frees all related resources. On error it is unsafe
 * to retry or use the publisher.
 *
 * Parameters:
 *      plugin     : Pointer of a plugin data structure
 *      subscriber : Pointer to a subscriber to be deleted.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
inline safuResultE_t elosPluginDeleteSubscriber(elosPlugin_t *const plugin, struct elosSubscriber *subscriber) {
    return plugin->deleteSubscriber(plugin->instanceRef, subscriber);
}

/*******************************************************************
 * Publish an event.
 *
 * Parameters:
 *      publisher : Pointer to elosPublisher data structure
 *      event     : Pointer to the event to publish. The event is deep copied
 *                  so it is save to call `elosEventDeleteMembers` on the event
 *                  after publishing.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
inline safuResultE_t elosPluginPublish(elosPlugin_t *const plugin, struct elosPublisher *publisher,
                                       const elosEvent_t *const event) {
    return plugin->publish(publisher, event);
}

/*******************************************************************
 * Store a given event. The caller keeps ownership of the event.
 *
 * Parameters:
 *      control : Pointer of a PluginControl data structure
 *      event   : Pointer to the event to store.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
inline safuResultE_t elosPluginStore(elosPlugin_t *const plugin, const elosEvent_t *const event) {
    return plugin->store(plugin->instanceRef, event);
}

/*******************************************************************
 * Do a log storage lookup of events for a given filter.
 *
 * Parameters:
 *      plugin : Pointer of a plugin data structure
 *      rule    : A string containing the event filter rule to be use to lookup
 *                historical events
 *      events  : A pointer to an initialised event vector to store the
 *                resulting events. The events in the vector must be freed by
 *                the caller by `elosEventDeleteMembers`.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
static inline safuResultE_t elosPluginFindEvents(elosPlugin_t *const plugin, const char *const rule, safuVec_t *events) {
    struct timespec newest = {0};
    struct timespec oldest = {0};
    return plugin->findEvents(plugin->instanceRef, rule, newest, oldest, events);
}

/*******************************************************************
 * Subscribe for a given list of filters.
 *
 * Parameters:
 *      subscriber    : Pointer of a Subscriber data structure
 *      filterStrings : A list of event filter string a subscription and event
 *                      queue shall be created for.
 *      filterCount   : Number of event filter strings provided.
 *      subscription  : A pointer to subscription pointer to store the new
 *                      created subscription. The pointer keeps unchanged on
 *                      failure. The ownership of the subscription remains by
 *                      the `PluginControl` instance.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
inline safuResultE_t elosPluginSubscribe(elosPlugin_t *const plugin, struct elosSubscriber *subscriber,
                                         char const *const *const filterStrings, const size_t filterCount,
                                         const elosSubscription_t **const subscription) {
    return plugin->subscribe(subscriber, filterStrings, filterCount, subscription);
}

/*******************************************************************
 * Fetch all events from an event queue, identified by given `subscription`.
 * The caller takes ownership of the returned event objects and is responsible
 * for correct disposal by calling `elosEventDeleteMembers` on them and for the
 * `eventVector`.
 *
 * Parameters:
 *      subscriber   : Pointer of a subscriber data structure
 *      subscription : The subscription for the event queue to be read.
 *      eventVector  : A pointer to to `safuVec_t` pointer to store the created
 *                     result vector in. The caller takes ownership of the
 *                     result vector and its elements.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
inline safuResultE_t elosPluginReadQueue(elosPlugin_t *const plugin, struct elosSubscriber *subscriber,
                                         const elosSubscription_t *const subscription, safuVec_t **const events) {
    return plugin->readQueue(subscriber, subscription, events);
}

/**
 * Cancel and cleanup a subscription create by the subscriber.
 *
 * Parameters:
 *     plugin: reference to the plugin
 *     subscriber: reference to the subcriber
 * Returns:
 *     - SAFU_RESULT_OK: Subscriptions are canceled succesfully
 *     - SAFU_RESULT_FAIL: Subscriptions couldn't be canceled or plugin is NULL.
 **/
inline safuResultE_t elosPluginUnsubscribe(elosPlugin_t *const plugin, struct elosSubscriber *subscriber,
                                           elosSubscription_t *const subscription) {
    return plugin->unsubscribe(subscriber, subscription);
}

/**
 * Cancel and cleanup all subscriptions of the given subscriber instance.
 *
 * Note:
 *     Tries to unsubscribe each subscription. It continues to unsubscribe if
 *     one subscription fails.
 * Parameters:
 *     plugin: reference to the plugin
 *     subscriber: reference to the subcriber to clear all subscriptions for
 * Returns:
 *     - SAFU_RESULT_OK: All subscriptions are canceled succesfully
 *     - SAFU_RESULT_FAIL: One or more subscriptions couldn't be canceled or plugin is NULL.
 **/
inline safuResultE_t elosPluginUnsubscribeAll(elosPlugin_t *const plugin, struct elosSubscriber *subscriber) {
    return plugin->unsubscribeAll(subscriber);
}

__END_DECLS
