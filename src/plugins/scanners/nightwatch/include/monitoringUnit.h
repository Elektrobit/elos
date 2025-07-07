// SPDX-License-Identifier: MIT

#ifndef MONITORINGUNIT_H
#define MONITORINGUNIT_H

#include "application.h"
#include "configKeys.h"
#include "thresholds.h"
#include <elos/libelosplugin/libelosplugin.h>
#include <safu/common.h>
#include <safu/log.h>
#include <vector>
#include <yaml-cpp/yaml.h>

class MonitoringUnit {
private:
  std::vector<std::unique_ptr<Application>> apps;
  unsigned int testInterval;
  Thresholds monitoringUnitThresholds;
  ConfigKeys configKeys;

public:
  /**
   * Construct a new Monitoring Unit object
   *
   * @param testInterval: test interval for checking the applications in
   * Monitoring Unit
   * @param thresholds: thresholds of the Monitoring Unit
   */
  MonitoringUnit(unsigned int testInterval, Thresholds thresholds);

  /**
   * Creates Application objects and add them to the Monitoring Unit
   *
   * @param appData: data of the application to be created
   * @param plugin: pointer of a plugin data structure
   * @param publisher: pointer to a publisher data structure
   */
  void addApps(YAML::Node appData, elosPlugin_t *plugin,
               elosPublisher *publisher);

  /**
   * Function to return the test interval of the Monitoring Unit
   *
   * @return unsigned int
   */
  unsigned int getTestInterval();

  /**
   * Function to call checkUsages of every application in Monitoring Unit
   */
  void runCheckingOfAppsUsages();
};

#endif
