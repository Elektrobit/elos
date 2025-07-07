// SPDX-License-Identifier: MIT

#include "punisher.h"

Punisher::Punisher(elosPlugin_t *plugin, elosSubscriber *subscriber)
    : plugin{plugin}, subscriber{subscriber}, subscription{}, events{nullptr},
      keepRunningEventHandling{}, eventHandlingThread{} {
  safuLogDebug("Initialize punisher");
  const char *filterString = ".event.source.appName 'nightwatch' STRCMP";
  safuResultE_t result = elosPluginSubscribe(
      plugin, subscriber, &filterString, 1, &subscription);
  if (result == SAFU_RESULT_FAILED) {
    safuLogErrF("Could not subscribe to event filter: '%s'", filterString);
  } else {
    safuLogDebugF("Could subscribe to event filter: '%s'", filterString);
  }
}

Punisher::~Punisher() {
  if (eventHandlingThread.joinable()) {
    eventHandlingThread.join();
  }
  safuResultE_t result =
      elosPluginUnsubscribeAll(plugin, subscriber);
  if (result == SAFU_RESULT_FAILED) {
    safuLogErr("Could not unsubscribe the subscribtion");
  }
}

safuResultE_t Punisher::reloadProc(const std::string &pid) {
  safuResultE_t result = SAFU_RESULT_FAILED;
  try {
    pid_t pidNum = std::stoi(pid);
    if (kill(pidNum, SIGHUP) == -1) {
      safuLogErr("Failed to send SIGHUP");
    } else {
      result = SAFU_RESULT_OK;
    }
  } catch (std::invalid_argument &e) {
    safuLogErr("No falid pid found");
  }
  return result;
}

safuResultE_t Punisher::killProc(const std::string &pid) {
  safuResultE_t result = SAFU_RESULT_FAILED;
  try {
    pid_t pidNum = std::stoi(pid);
    if (kill(pidNum, SIGKILL) == -1) {
      safuLogErr("Failed to send SIGKILL");
    } else {
      result = SAFU_RESULT_OK;
    }
  } catch (std::invalid_argument &e) {
    safuLogErr("No falid pid found");
  }
  return result;
}

safuResultE_t Punisher::fetchProcID(const std::string &payload,
                                    std::string &pid) {
  safuResultE_t result = SAFU_RESULT_FAILED;
  std::stringstream ss(payload);
  std::string item;

  while (std::getline(ss, item, ';')) {
    if (item.find("pid:") == 0) {
      pid = item.substr(4);
      result = SAFU_RESULT_OK;
      break;
    }
  }
  if (pid.empty()) {
    result = SAFU_RESULT_FAILED;
  }
  return result;
}

safuResultE_t Punisher::fetchProcName(const std::string &payload,
                                      std::string &name) {
  safuResultE_t result = SAFU_RESULT_FAILED;
  std::stringstream ss(payload);
  std::string item;

  while (std::getline(ss, item, ';')) {
    if (item.find("name:") == 0) {
      name = item.substr(5);
      result = SAFU_RESULT_OK;
      break;
    }
  }
  return result;
}

safuResultE_t Punisher::handleEvent(const elosEvent_t *const event) {
  safuResultE_t resultHandleEvent = SAFU_RESULT_FAILED;
  safuResultE_t result = SAFU_RESULT_FAILED;
  elosSeverityE_t severity = event->severity;
  elosEventMessageCodeE_t messageCode = event->messageCode;
  std::string payload = event->payload;
  std::string pid = "";

  result = fetchProcID(payload, pid);
  if (result == SAFU_RESULT_FAILED) {
    safuLogErr("Could not find process id");
    return resultHandleEvent;
  }
  std::string name = "";
  result = fetchProcName(payload, name);
  if (result == SAFU_RESULT_FAILED) {
    safuLogWarn("Could not find process name");
  }

  if (event != nullptr && messageCode == ELOS_MSG_CODE_RESOURCE_OVERLOAD) {
    switch (severity) {
    case ELOS_SEVERITY_INFO:
      safuLogDebugF("Process '%s' is in Threshold. No action taken",
                    name.c_str());
      resultHandleEvent = SAFU_RESULT_OK;
      break;
    case ELOS_SEVERITY_WARN:
      safuLogDebugF("Process '%s' reached Threshold 1. No action taken",
                    name.c_str());
      resultHandleEvent = SAFU_RESULT_OK;
      break;
    case ELOS_SEVERITY_ERROR:
      safuLogDebugF("Process '%s' reached Threshold 2. Restart process",
                    name.c_str());
      resultHandleEvent = reloadProc(pid);
      break;
    case ELOS_SEVERITY_FATAL:
      safuLogDebugF("Process '%s' reached Threshold 3. Kill process",
                    name.c_str());
      resultHandleEvent = killProc(pid);
      break;
    default:
      safuLogWarn("Severity does not match for punisher. Skipped event");
      resultHandleEvent = SAFU_RESULT_OK;
    }
  } else {
    safuLogErr("Event does not match format");
  }
  return resultHandleEvent;
}

safuResultE_t Punisher::handleQueue() {
  safuResultE_t resultQueue = SAFU_RESULT_OK;
  uint32_t numberOfEvents = safuVecElements(events);
  for (uint32_t i = 0; i < numberOfEvents; i++) {
    void *genericPointer = safuVecGetLast(events);
    elosEvent_t *event = static_cast<elosEvent_t *>(genericPointer);
    safuResultE_t result = handleEvent(event);

    if (result == SAFU_RESULT_FAILED) {
      safuLogErr("Failed to correctly handle event");
      resultQueue = SAFU_RESULT_FAILED;
    }
    result = elosEventDeleteMembers(event);
    if (result == SAFU_RESULT_FAILED) {
      safuLogErr("Failed to delete members of event");
      resultQueue = SAFU_RESULT_FAILED;
    } else {
      safuVecPop(events);
    }
  }
  if (!safuVecElements(events)) {
    safuVecFree(events);
  }
  free(events);
  return resultQueue;
}

void Punisher::runEventHandling() {
  while (keepRunningEventHandling) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    result = elosPluginReadQueue(plugin, subscriber,
                                 subscription, &events);
    if (result == SAFU_RESULT_FAILED) {
      safuLogErr("Could not read event queue");
    } else {
      result = handleQueue();
      if (result == SAFU_RESULT_FAILED) {
        safuLogErr("Could not handle queue correctly");
      }
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void Punisher::runEventHandlingThread() {
  safuLogDebug("Run Event Handling");
  keepRunningEventHandling = true;
  eventHandlingThread = std::thread(&Punisher::runEventHandling, this);
}

void Punisher::stopEventHandlingThread() {
  safuLogDebug("Stop Event Handling");
  keepRunningEventHandling = false;
  if (eventHandlingThread.joinable()) {
    eventHandlingThread.join();
  }
}
