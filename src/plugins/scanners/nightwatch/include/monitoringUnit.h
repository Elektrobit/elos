// SPDX-License-Identifier: MIT

#ifndef MONITORINGUNIT_H
#define MONITORINGUNIT_H

#include <yaml-cpp/yaml.h>

#include <vector>

#include "application.h"
#include "configKeys.h"

class MonitoringUnit {
   private:
    std::vector<std::unique_ptr<Application>> apps;
    unsigned int testingInterval;
    elosThresholds monitoringUnitThresholds;
    elosConfigKeys configKeys;

   public:
    /**
     * Construct a new Monitoring Unit object
     *
     * @param testInterval: test interval for checking the applications in
     * Monitoring Unit
     * @param thresholds: thresholds of the Monitoring Unit
     */
    MonitoringUnit(unsigned int testInterval, elosThresholds thresholds);

    /**
     * Creates Application objects and add them to the Monitoring Unit
     *
     * @param appData: data of the application to be created
     * @param plugin: pointer of a plugin data structure
     * @param publisher: pointer to a publisher data structure
     */
    void addApps(YAML::Node appData, elosPlugin_t *plugin, elosPublisher *publisher);

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
