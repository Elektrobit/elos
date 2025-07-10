// SPDX-License-Identifier: MIT

#include "monitoringUnit.h"

#include <safu/log.h>

MonitoringUnit::MonitoringUnit(unsigned int testInterval, elosThresholds thresholds)
    : apps{}, testingInterval{testInterval}, monitoringUnitThresholds{thresholds}, configKeys{} {}

unsigned int MonitoringUnit::getTestInterval() {
    return testingInterval;
}

void MonitoringUnit::addApps(YAML::Node appsData, elosPlugin_t *plugin, elosPublisher *publisher) {
    std::vector<std::string> configThresholdKeys = {configKeys.memthreshold1, configKeys.memthreshold2,
                                                    configKeys.memthreshold3, configKeys.cputhreshold1,
                                                    configKeys.cputhreshold2, configKeys.cputhreshold3};
    if (!appsData) {
        safuLogErr("No apps data found. Check if the config file has applications key!");
    }
    if (appsData.IsNull()) {
        safuLogErr(
            "No app data found. Check if the config file is missing data of "
            "app in the monitoring unit!");
    }
    std::vector<float> defaultAppThresholds = {
        monitoringUnitThresholds.memThreshold1, monitoringUnitThresholds.memThreshold2,
        monitoringUnitThresholds.memThreshold3, monitoringUnitThresholds.cpuThreshold1,
        monitoringUnitThresholds.cpuThreshold2, monitoringUnitThresholds.cpuThreshold3};
    for (YAML::const_iterator iterator = appsData.begin(); iterator != appsData.end(); ++iterator) {
        std::string appKey = iterator->first.as<std::string>();
        YAML::Node appData = appsData[appKey];
        unsigned long maxMemUsage;
        double maxCpuLoad;
        std::vector<float> thresholds = defaultAppThresholds;
        if (appData[configKeys.maxmemusage]) {
            maxMemUsage = appData[configKeys.maxmemusage].as<unsigned long>();
        } else {
            safuLogErrF(
                "Key in config of application '%s' not found: '%s'. Need to "
                "be added to the config!.",
                appKey.c_str(), configKeys.maxmemusage.c_str());
        }
        if (appData[configKeys.maxcpuload]) {
            maxCpuLoad = appData[configKeys.maxcpuload].as<double>();
        } else {
            safuLogErrF(
                "Key in config of application '%s' not found: '%s'. Need to "
                "be added to the config!.",
                appKey.c_str(), configKeys.maxcpuload.c_str());
        }
        // reads config to get value of thresholds; index 0 - 5 for 6 thresholds
        for (std::size_t index = 0; index < 6; index++) {
            // set threshold from config
            if (appData[configThresholdKeys[index]]) {
                thresholds[index] = appData[configThresholdKeys[index]].as<float>();
            } else {
                safuLogWarnF(
                    "Key in config of application '%s' not found: '%s'. Use "
                    "default value.",
                    appKey.c_str(), configThresholdKeys[index].c_str());
            }
        }
        elosThresholds appThresholds = {thresholds[0], thresholds[1], thresholds[2],
                                        thresholds[3], thresholds[4], thresholds[5]};
        std::unique_ptr<Application> app =
            std::make_unique<Application>(plugin, publisher, appKey, appThresholds, maxMemUsage, maxCpuLoad);
        apps.push_back(std::move(app));
    }
}

void MonitoringUnit::runCheckingOfAppsUsages() {
    for (std::unique_ptr<Application> &app : apps) {
        app->checkUsages();
    }
}
