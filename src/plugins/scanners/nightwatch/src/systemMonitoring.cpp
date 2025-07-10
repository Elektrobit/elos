// SPDX-License-Identifier: MIT

#include "systemMonitoring.h"

#include <safu/log.h>

SystemMonitoring::SystemMonitoring(elosPlugin_t *plugin, elosPublisher *publisher, const std::string &configFilePath)
    : elosPlugin{plugin},
      elosPblisher{publisher},
      yamlData{},
      configKeys{},
      monitoringUnits{},
      testInterval{600},
      maxTestInterval{0},
      systemMonitoringThresholds{},
      keepRunningMonitoring{},
      monitoringThread{} {
    safuLogDebug("Initialize system monitoring setup");
    // loads the file and throws an error if it failed to load it
    try {
        yamlData = YAML::LoadFile(configFilePath);
        safuLogDebugF("Loaded file successfully: '%s'", configFilePath.c_str());
    } catch (const YAML::BadFile &e) {
        safuLogErrF("Failed to load file: '%s'", configFilePath.c_str());
        return;
    }
    setValuesOfSystemMonitoring();
    addMonitoringUnits();
}

SystemMonitoring::~SystemMonitoring() {
    if (monitoringThread.joinable()) {
        monitoringThread.join();
    }
}

void SystemMonitoring::setValuesOfSystemMonitoring() {
    // set testInterval of SystemMonitoring for default value for every monitoring
    // unit
    if (yamlData[configKeys.systemmonitoring][configKeys.testinterval]) {
        testInterval = yamlData[configKeys.systemmonitoring][configKeys.testinterval].as<unsigned int>();
    } else {
        safuLogWarnF("Key in config of systemmonitoring not found: '%s'. Use default value.",
                     configKeys.testinterval.c_str());
    }

    std::vector<float> thresholds = {1.00, 1.50, 2.00, 1.00, 1.50, 2.00};
    std::vector<std::string> configThresholdKeys = {configKeys.memthreshold1, configKeys.memthreshold2,
                                                    configKeys.memthreshold3, configKeys.cputhreshold1,
                                                    configKeys.cputhreshold2, configKeys.cputhreshold3};
    // reads config to get value of thresholds; index 0 - 5 for 6 thresholds
    for (std::size_t index = 0; index < 6; index++) {
        // set threshold from config
        if (yamlData[configKeys.systemmonitoring][configThresholdKeys[index]]) {
            thresholds[index] = yamlData[configKeys.systemmonitoring][configThresholdKeys[index]].as<float>();
        } else {
            safuLogWarnF(
                "Key in config of systemmonitoring not found: '%s'. Use "
                "default value.",
                configThresholdKeys[index].c_str());
        }
    }
    systemMonitoringThresholds = {thresholds[0], thresholds[1], thresholds[2],
                                  thresholds[3], thresholds[4], thresholds[5]};
}

void SystemMonitoring::addMonitoringUnits() {
    std::vector<std::string> configThresholdKeys = {configKeys.memthreshold1, configKeys.memthreshold2,
                                                    configKeys.memthreshold3, configKeys.cputhreshold1,
                                                    configKeys.cputhreshold2, configKeys.cputhreshold3};
    YAML::Node monitoringData = yamlData[configKeys.systemmonitoring][configKeys.monitoring];
    if (!monitoringData) {
        safuLogErr(
            "No monitoring data found. Check if the config file has "
            "monitoring key!");
    }
    if (monitoringData.IsNull()) {
        safuLogErr(
            "No monitoring data found. Check if the config file is missing "
            "data of monitoring unit!");
    }
    std::vector<float> defaultMonitoringUnitThresholds = {
        systemMonitoringThresholds.memThreshold1, systemMonitoringThresholds.memThreshold2,
        systemMonitoringThresholds.memThreshold3, systemMonitoringThresholds.cpuThreshold1,
        systemMonitoringThresholds.cpuThreshold2, systemMonitoringThresholds.cpuThreshold3};
    for (YAML::const_iterator iterator = monitoringData.begin(); iterator != monitoringData.end(); ++iterator) {
        std::string monitoringUnitKey = iterator->first.as<std::string>();
        YAML::Node monitoringUnitData = monitoringData[monitoringUnitKey];
        unsigned int monitoringUnitTestInterval;
        std::vector<float> thresholds = defaultMonitoringUnitThresholds;
        if (monitoringUnitData[configKeys.testinterval]) {
            monitoringUnitTestInterval = monitoringUnitData[configKeys.testinterval].as<unsigned int>();
        } else {
            safuLogWarnF(
                "Key in config of monitoring unit '%s' not found: '%s'. Use "
                "default value.",
                monitoringUnitKey.c_str(), configKeys.testinterval.c_str());
            monitoringUnitTestInterval = testInterval;
        }
        // reads config to get value of thresholds; index 0 - 5 for 6 thresholds
        for (std::size_t index = 0; index < 6; index++) {
            // set threshold from config
            if (monitoringUnitData[configThresholdKeys[index]]) {
                thresholds[index] = monitoringUnitData[configThresholdKeys[index]].as<float>();
            } else {
                safuLogWarnF(
                    "Key in config of monitoring unit '%s' not found: '%s'. "
                    "Use default value.",
                    monitoringUnitKey.c_str(), configThresholdKeys[index].c_str());
            }
        }
        elosThresholds monitoringUnitThresholds = {thresholds[0], thresholds[1], thresholds[2],
                                                   thresholds[3], thresholds[4], thresholds[5]};
        std::unique_ptr<MonitoringUnit> monitoringUnit =
            std::make_unique<MonitoringUnit>(monitoringUnitTestInterval, monitoringUnitThresholds);
        YAML::Node appsData = monitoringUnitData[configKeys.apps];
        monitoringUnit->addApps(appsData, elosPlugin, elosPblisher);
        monitoringUnits.push_back(std::move(monitoringUnit));
    }
}

void SystemMonitoring::runMonitoring() {
    std::vector<std::chrono::steady_clock::time_point> lastRunTimes(monitoringUnits.size(),
                                                                    std::chrono::steady_clock::now());
    while (keepRunningMonitoring) {
        auto now = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < monitoringUnits.size(); ++i) {
            auto &monitoringUnit = monitoringUnits[i];
            auto interval = std::chrono::milliseconds(monitoringUnit->getTestInterval());

            if (now - lastRunTimes[i] >= interval) {
                monitoringUnit->runCheckingOfAppsUsages();
                lastRunTimes[i] = now;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void SystemMonitoring::runMonitoringThread() {
    safuLogDebug("Run Monitoring");
    keepRunningMonitoring = true;
    if (maxTestInterval == 0) {
        for (const std::unique_ptr<MonitoringUnit> &monitoringUnit : monitoringUnits) {
            if (maxTestInterval < monitoringUnit->getTestInterval()) {
                maxTestInterval = monitoringUnit->getTestInterval();
            }
        }
    }
    monitoringThread = std::thread(&SystemMonitoring::runMonitoring, this);
}

void SystemMonitoring::stopMonitoringThread() {
    safuLogDebug("Stop Monitoring");
    keepRunningMonitoring = false;
    if (monitoringThread.joinable()) {
        monitoringThread.join();
    }
}
