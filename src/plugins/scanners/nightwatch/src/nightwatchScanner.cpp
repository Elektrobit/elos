// SPDX-License-Identifier: MIT

#include "systemMonitoring.h"
#include <elos/libelosplugin/libelosplugin.h>
#include <elos/libelosplugin/types.h>
#include <new>
#include <safu/common.h>
#include <safu/log.h>
#include <stdlib.h>

SystemMonitoring *sysMonitor;
elosPublisher *publisher;

static safuResultE_t _pluginLoad(elosPlugin_t *plugin) {
  safuResultE_t result = SAFU_RESULT_FAILED;

  if (plugin == NULL) {
    safuLogErr("Null parameter given");
  } else {
    result = elosPluginCreatePublisher(plugin, &publisher);
    if (result == SAFU_RESULT_OK) {
      sysMonitor =
          new (std::nothrow) SystemMonitoring(plugin, publisher, "/etc/elos/nightwatch/config.yaml");
    } else {
      safuLogErr("Could not create publisher");
    }

    if (sysMonitor == nullptr) {
      safuLogErr("Allocating system monitoring failed!");
    } else if ((plugin->config == NULL) || (plugin->config->key == NULL)) {
      safuLogErr("Given configuration is NULL or has .key set to NULL");
    } else {
      plugin->data = NULL;

      safuLogDebugF("Scanner Plugin '%s' has been loaded", plugin->config->key);
      result = SAFU_RESULT_OK;
    }
  }

  return result;
}

static safuResultE_t _pluginStart(elosPlugin_t *plugin) {
  safuResultE_t result = SAFU_RESULT_FAILED;

  if (plugin == NULL) {
    safuLogErr("Null parameter given");
  } else {
    safuLogDebugF("Scanner Plugin '%s' has been started", plugin->config->key);

    if (sysMonitor != nullptr) {
      sysMonitor->runMonitoringThread();
    }

    result = elosPluginReportAsStarted(plugin);
    if (result == SAFU_RESULT_FAILED) {
      safuLogErr("elosPluginReportAsStarted failed");
    } else {
      result = elosPluginStopTriggerWait(plugin);
      if (result == SAFU_RESULT_FAILED) {
        safuLogErr("elosPluginStopTriggerWait failed");
      }
    }
  }

  return result;
}

static safuResultE_t _pluginStop(elosPlugin_t *plugin) {
  safuResultE_t result = SAFU_RESULT_FAILED;

  if (plugin == NULL) {
    safuLogErr("Null parameter given");
  } else {
    safuLogDebugF("Stopping Scanner Plugin '%s'", plugin->config->key);

    if (sysMonitor != nullptr) {
      sysMonitor->stopMonitoringThread();
    }

    result = elosPluginStopTriggerWrite(plugin);
    if (result == SAFU_RESULT_FAILED) {
      safuLogErr("elosPluginStopTriggerWrite failed");
    }
  }

  return result;
}

static safuResultE_t _pluginUnload(elosPlugin_t *plugin) {
  safuResultE_t result = SAFU_RESULT_FAILED;

  if (plugin == NULL) {
    safuLogErr("Null parameter given");
  } else {
    safuLogDebugF("Unloading Scanner Plugin '%s'", plugin->config->key);
    free(plugin->data);
    delete sysMonitor;
    result = elosPluginDeletePublisher(plugin, publisher);
    if (result == SAFU_RESULT_FAILED) {
      safuLogErr("Failed to delete publisher");
    }
  }

  return result;
}

__BEGIN_DECLS

elosPluginConfig_t elosPluginConfig = {
    PLUGIN_TYPE_SCANNER, _pluginLoad, _pluginUnload, _pluginStart, _pluginStop,
};

__END_DECLS
