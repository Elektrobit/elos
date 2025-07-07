// SPDX-License-Identifier: MIT

#ifndef PUNISHER_H
#define PUNISHER_H

#include <atomic>
#include <chrono>
#include <elos/event/event.h>
#include <elos/event/event_types.h>
#include <elos/libelosplugin/libelosplugin.h>
#include <elos/libelosplugin/types.h>
#include <fstream>
#include <safu/common.h>
#include <safu/log.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <thread>

class Punisher {
private:
  elosPlugin_t *plugin;
  elosSubscriber *subscriber;
  const elosSubscription_t *subscription;
  safuVec_t *events = nullptr;
  std::atomic<bool> keepRunningEventHandling;
  std::thread eventHandlingThread;

  /**
   * Function to reload a process
   *
   * @param pid: process id of process to be reloaded
   * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
   * on failure
   */
  safuResultE_t reloadProc(const std::string &pid);

  /**
   * Function to kill a process
   *
   * @param pid: process id of process to be killed
   * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
   * on failure
   */
  safuResultE_t killProc(const std::string &pid);

  /**
   * Function to fetch the process id of the payload of the elos event send by
   * nightwatch
   *
   * @param payload: payload of the event send by nightwatch
   * @param pid: variable to safe to the process id
   * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
   * on failure
   */
  safuResultE_t fetchProcID(const std::string &payload, std::string &pid);

  /**
   * Function to fetch the process name of the payload of the elos event send by
   * nightwatch
   *
   * @param payload: payload of the event send by nightwatch
   * @param name: variable to safe to the process name
   * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
   * on failure
   */
  safuResultE_t fetchProcName(const std::string &payload, std::string &name);

  /**
   * Function to handle the event
   *
   * Fetches data of the event and decides which action should be taken
   *
   * @param event: pointer to an event data structure
   * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
   * on failure
   */
  safuResultE_t handleEvent(const elosEvent_t *const event);

  /**
   * Function to handle the event queue identified by given subscription
   *
   * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
   * on failure
   */
  safuResultE_t handleQueue();

  /**
   * Function to run the event handling
   */
  void runEventHandling();

public:
  /**
   * Construct a new Punisher object
   *
   * @param plugin: pointer of a plugin data structure
   * @param subscriber: pointer to a subscriber data structure
   */
  Punisher(elosPlugin_t *plugin, elosSubscriber *subscriber);

  /**
   * Destroy the Punisher object
   *
   * Function to destroy the Punisher object, it will also unsubscribe all
   * subscriptions
   */
  ~Punisher();

  /**
   * Function to start the event handling thread
   */
  void runEventHandlingThread();

  /**
   * Function to stop the event handling thread
   */
  void stopEventHandlingThread();
};

#endif
