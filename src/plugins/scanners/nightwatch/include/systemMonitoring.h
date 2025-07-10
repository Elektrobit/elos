// SPDX-License-Identifier: MIT

#ifndef SYSTEMMONITORING_H
#define SYSTEMMONITORING_H

#include <thread>

#include "monitoringUnit.h"

class SystemMonitoring {
   private:
    elosPlugin_t *elosPlugin;
    elosPublisher *elosPblisher;
    YAML::Node yamlData;
    const elosConfigKeys configKeys;
    std::vector<std::unique_ptr<MonitoringUnit>> monitoringUnits;
    unsigned int testInterval;
    unsigned int maxTestInterval;
    elosThresholds systemMonitoringThresholds;
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
    SystemMonitoring(elosPlugin_t *plugin, elosPublisher *publisher, const std::string &configFilePath);

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
