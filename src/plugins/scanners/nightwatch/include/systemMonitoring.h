// SPDX-License-Identifier: MIT

#ifndef SYSTEMMONITORING_H
#define SYSTEMMONITORING_H

#include "configKeys.h"
#include "monitoringUnit.h"
#include "thresholds.h"
#include <atomic>
#include <chrono>
#include <elos/libelosplugin/types.h>
#include <filesystem>
#include <iostream>
#include <safu/common.h>
#include <safu/log.h>
#include <string>
#include <thread>
#include <vector>
#include <yaml-cpp/yaml.h>

class SystemMonitoring {
private:
  elosPlugin_t *plugin;
  elosPublisher *publisher;
  YAML::Node yamlData;
  const ConfigKeys configKeys;
  std::vector<std::unique_ptr<MonitoringUnit>> monitoringUnits;
  unsigned int testInterval;
  unsigned int maxTestInterval;
  Thresholds systemMonitoringThresholds;
  std::atomic<bool> keepRunningMonitoring;
  std::thread monitoringThread;

  /**
   * Set the Values Of System Monitoring object
   */
  void setValuesOfSystemMonitoring();

  /**
   * Creates Monitoring Unit objects and add them to the System Monitoring
   */
  void addMonitoringUnits();

  /**
   * Function to run the monitoring
   */
  void runMonitoring();

public:
  /**
   * Construct a new System Monitoring object
   *
   * @param plugin: pointer of a plugin data structure
   * @param publisher: pointer to a publisher data structure
   * @param configFilename: name of the config file
   */
  SystemMonitoring(elosPlugin_t *plugin, elosPublisher *publisher,
                   const std::string &configFilePath);

  /**
   * Destroy the System Monitoring object
   *
   * Function to destroy the System Monitoring
   */
  ~SystemMonitoring();

  /**
   * Function to start the monitoring thread
   */
  void runMonitoringThread();

  /**
   * Function to stop the monitoring thread
   */
  void stopMonitoringThread();
};

#endif
