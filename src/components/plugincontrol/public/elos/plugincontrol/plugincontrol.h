// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/defines.h"
#include "elos/plugincontrol/types.h"

__BEGIN_DECLS

/*******************************************************************
 * Initializes an existing PluginControl data structure
 *
 * Parameters:
 *      control : Pointer to the PluginControl data structure
 *      param   : Parameters for initialization
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlInitialize(elosPluginControl_t *control, elosPluginControlParam_t const *param);

/*******************************************************************
 * Allocates and initializes a PluginControl data structure
 *
 * Parameters:
 *      control : Pointer to a pointer that will contain
 *                the new PluginControl data structure
 *      param   : Parameters for initialization
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlNew(elosPluginControl_t **control, elosPluginControlParam_t const *param);

/*******************************************************************
 * Loads the Plugin shared object specified during initialization
 * and checks if everything needed is present, as well as the
 * type of the Plugin itself (Scanner, Storage, ...).
 * Also starts a worker thread and calls the Plugins 'load' function,
 * the worker thread will sleep until elosPluginControlStart is called.
 * The function call will return once the worker thread is ready,
 * so no further synchronization is needed.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already loaded
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlLoad(elosPluginControl_t *control);

/*******************************************************************
 * Unloads the Plugin by calling its 'unload' function,
 * closing the worker thread and freeing all memory associated with it afterwards.
 * Will call elosPluginControlStop first if the Plugin is still running.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already unloaded
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlUnload(elosPluginControl_t *control);

/*******************************************************************
 * Starts the Plugin by activating the Plugins worker thread
 * and calling the Plugins 'start' function. Returns when the Plugin
 * is running, so no further synchronization is needed.
 * The Plugin needs to be loaded for this command to work.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already started
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlStart(elosPluginControl_t *control);

/*******************************************************************
 * Stops the Plugin via the Plugins 'stop' function.
 * The Plugin needs to be loaded for this command to work.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already stopped
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlStop(elosPluginControl_t *control);

/*******************************************************************
 * Frees all member data of an PluginControl data structure.
 * Will call elosPluginControlUnload first if the Plugin is still loaded.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already deleted
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlDeleteMembers(elosPluginControl_t *control);

/*******************************************************************
 * Frees all member data of an PluginControl data structure by calling
 * elosPluginControlDeleteMembers, then frees the data structure itself.
 *
 * Parameters:
 *      control : Pointer to a pointer of an PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already deleted
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlDelete(elosPluginControl_t **control);

/*******************************************************************
 * Get the Plugins name based on the configuration associated with it.
 * The returned name string will be still associated with the samconf
 * data structure and must not be freed to avoid problems.
 *
 * Parameters:
 *      control : Pointer of a PluginControl data structure
 *      name    : Pointer to a pointer to the string containing the name
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlGetName(const elosPluginControl_t *control, const char **name);

/******************************************************************************
 * Implements `pluginCreatePublisher` interface. A new publisher will be
 * allocated and stored in the provided pointer. On any failure the publisher
 * pointer remains unchanged.
 *
 * Parameters:
 *      control   : Pointer of a PluginControl data structure
 *      publisher : Pointer to a pointer where to store the pointer of the new
 *                  publisher in case of success, otherwise it remains unchanged
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
safuResultE_t elosPluginControlCreatePublisher(struct elosPluginControl *pluginControl,
                                               struct elosPublisher **publisher);

/******************************************************************************
 * Implements `pluginDeletePublisher` interface. Deletes a publisher and frees
 * all related resources. On error it is unsafe to retry or use the publisher.
 *
 * Parameters:
 *      control   : Pointer of a PluginControl data structure
 *      publisher : Pointer to a publisher to be deleted.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
safuResultE_t elosPluginControlDeletePublisher(struct elosPluginControl *pluginControl,
                                               struct elosPublisher *publisher);

/******************************************************************************
 *
 * Implements `pluginCreateSubscriber` interface. A new publisher will be
 * allocated and stored in the provided pointer. On any failure the publisher
 * pointer remains unchanged.
 *
 * Parameters:
 *      control    : Pointer of a PluginControl data structure
 *      subscriber : Pointer to a pointer where to store the pointer of the new
 *                   subscriber in case of success, otherwise it remains
 *                   unchanged
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
safuResultE_t elosPluginControlCreateSubscriber(struct elosPluginControl *pluginControl,
                                                struct elosSubscriber **subscriber);

/******************************************************************************
 * Implements `pluginDeleteSubscriber` interface. Deletes a publisher and frees
 * all related resources. On error it is unsafe to retry or use the publisher.
 *
 * Parameters:
 *      control   : Pointer of a PluginControl data structure
 *      subscriber : Pointer to a subscriber to be deleted.
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 *****************************************************************************/
safuResultE_t elosPluginControlDeleteSubscriber(struct elosPluginControl *pluginControl,
                                                struct elosSubscriber *subscriber);

/*******************************************************************
 * Implements the actual publishing procedure of an event.
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
safuResultE_t elosPluginControlPublish(elosPublisher_t *publisher, const elosEvent_t *event);

/*******************************************************************
 * Implements the actual storage lookup procedure of events for a given filter.
 *
 * Parameters:
 *      control : Pointer of a PluginControl data structure
 *      rule    : A string containing the event filter rule to be use to lookup
 *                historical events
 *      events  : A pointer to an initialised event vector to store the
 *                resulting events. The events in the vector must be freed by
 *                the caller by `elosEventDeleteMembers`.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlFindEvents(elosPluginControl_t *pluginControl, const char *rule,
                                          struct timespec const *newest, struct timespec const *oldest,
                                          safuVec_t *events);

/*******************************************************************
 * Implements the actual retrieval of all events from an event queue,
 * identified by given `elosSubscription_t`. The caller takes ownership of the
 * returned event objects and is responsible for correct disposal by calling
 * `elosEventDeleteMembers` on them and for the `eventVector`.
 *
 * Parameters:
 *      subscriber   : Pointer of a subscriber data structure
 *      eventQueueId : The event queue to be read.
 *      eventVector  : A pointer to to `safuVec_t` pointer to store the created
 *                     result vector in. The caller takes ownership of the
 *                     result vector and its elements.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlEventQueueRead(elosSubscriber_t *subscriber, const elosSubscription_t *subscription,
                                              safuVec_t **const eventVector);

/*******************************************************************
 * Implements the actual storage procedure of a given event. The caller keeps
 * ownership of the event.
 *
 * Parameters:
 *      control : Pointer of a PluginControl data structure
 *      event   : Pointer to the event to store.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlStore(elosPluginControl_t *pluginControl, const elosEvent_t *event);

/*******************************************************************
 * Implements the actual subscription process for a given list of filters.
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
safuResultE_t elosPluginControlSubscribe(elosSubscriber_t *subscriber, char const *const *filterStrings,
                                         size_t filterCount, const elosSubscription_t **const subscription);

/*******************************************************************
 * Implement the actual process to cancel an active subscription.
 *
 * Parameters:
 *      subscriber   : Pointer of a Subscriber data structure
 *      subscription : Pointer to a subscription object describing the
 *                     subscription which shall be canceled.
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlUnsubscribe(elosSubscriber_t *subscriber, elosSubscription_t const *const subscription);

/*******************************************************************
 * Implement a way to cancel a all subscriptions maintained by the given plugin
 * instance.
 *
 * Parameters:
 *      subscriber : Pointer of a Subscriber data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlUnsubscribeAll(elosSubscriber_t *subscriber);

__END_DECLS
