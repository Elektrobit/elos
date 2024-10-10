// SPDX-License-Identifier: MIT
#pragma once

#include <elos/common/types.h>
#include <elos/event/types.h>
#include <safu/flags.h>
#include <safu/result.h>
#include <samconf/samconf.h>

#define ELOS_PLUGIN_ID_INVALID 0

__BEGIN_DECLS

typedef uint32_t elosPluginId_t;
typedef uint32_t elosPluginState_t;

typedef enum elosPluginStateE {
    PLUGIN_STATE_INVALID = 0,
    PLUGIN_STATE_INITIALIZED,
    PLUGIN_STATE_LOADED,
    PLUGIN_STATE_STARTED,
    PLUGIN_STATE_STOPPED,
    PLUGIN_STATE_UNLOADED,
    PLUGIN_STATE_ERROR,
} elosPluginStateE_t;

/*
 * Forward declaration of the plugin control interface implemented by elosd.
 * */
struct elosPluginControl;
struct elosPublisher;
struct elosSubscriber;

typedef struct elosPluginContext {
    samconfConfig_t const *config;
    bool useEnv;
    elosPluginId_t id;
    void *data;
    int sync;
    int stop;
    struct elosPluginControl *instanceRef;
    safuResultE_t (*publish)(struct elosPublisher *publisher, const elosEvent_t *event);
    safuResultE_t (*store)(struct elosPluginControl *instanceRef, const elosEvent_t *event);
    safuResultE_t (*findEvents)(struct elosPluginControl *instanceRef, const char *rule, struct timespec const *newest,
                                struct timespec const *oldest, safuVec_t *events);
    safuResultE_t (*subscribe)(struct elosSubscriber *subscriber, char const *const *filterStrings, size_t filterCount,
                               const elosSubscription_t **const subscription);
    safuResultE_t (*readQueue)(struct elosSubscriber *subscriber, const elosSubscription_t *subscription,
                               safuVec_t **events);
    safuResultE_t (*unsubscribe)(struct elosSubscriber *subscriber, const elosSubscription_t *subscription);
    safuResultE_t (*unsubscribeAll)(struct elosSubscriber *subscriber);
    safuResultE_t (*createPublisher)(struct elosPluginControl *instanceRef, struct elosPublisher **publisher);
    safuResultE_t (*deletePublisher)(struct elosPluginControl *instanceRef, struct elosPublisher *publisher);
    safuResultE_t (*createSubscriber)(struct elosPluginControl *instanceRef, struct elosSubscriber **subscriber);
    safuResultE_t (*deleteSubscriber)(struct elosPluginControl *instanceRef, struct elosSubscriber *subscriber);
} elosPlugin_t;

typedef safuResultE_t(elosPluginFunc_t)(elosPlugin_t *);

/*******************************************************************
 * Selects the type of the plugin.
 ******************************************************************/
typedef enum elosPluginTypeE {
    PLUGIN_TYPE_INVALID = 0,
    PLUGIN_TYPE_SCANNER,
    PLUGIN_TYPE_STORAGEBACKEND,
    PLUGIN_TYPE_CLIENTCONNECTION,
} elosPluginTypeE_t;

/*******************************************************************
 * Configuration for a plugin. Sets the type of the plugin as well
 * as the functions that are used by PluginControl.
 *
 * Must always have the variable name "elosPluginConfig" so
 * PluginControl can locate it when the plugin is loaded.
 *
 * Members:
 *  type: Defines the type of the plugin (scanner, ...)
 *  load: The function to be used to set up (initialize) the plugins
 *  unload: The function to be used to teardown (free) the plugin
 *  start: The function for starting the plugin itself
 *  stop: The function for stopping a started plugin
 ******************************************************************/
typedef struct elosPluginConfig {
    elosPluginTypeE_t type;
    elosPluginFunc_t *load;
    elosPluginFunc_t *unload;
    elosPluginFunc_t *start;
    elosPluginFunc_t *stop;
} elosPluginConfig_t;

__END_DECLS
