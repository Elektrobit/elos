// SPDX-License-Identifier: MIT

#ifndef APPLICATION_H
#define APPLICATION_H

#include <elos/libelosplugin/libelosplugin.h>

#include <string>

#include "thresholds.h"

class Application {
   private:
    elosPlugin_t *elosPlugin;
    elosPublisher *elosPblisher;
    std::string appName;
    std::string pid;
    unsigned long memUsageKb;
    double cpuLoadPer;
    elosThresholds applicationThresholds;
    unsigned long maxMemoryUsage;
    double maxCPULoad;

    /**
     * Executes the given command and returns the command output
     *
     * @param cmd: command to be executed
     * @return std::string - command output, empty string on failure
     */
    std::string execCmd(const char *cmd);

    /**
     * Assign the current memory usage of the application to memUsageKb
     *
     * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
     * on failure
     */
    safuResultE_t assignMemUsage();

    /**
     * Assign the current cpu laod of the application to cpuLoadPer
     *
     * @return safuResultE_t - 'SAFU_RESULT_OK' on success, 'SAFU_RESULT_FAILED'
     * on failure
     */
    safuResultE_t assignCpuLoad();

    /**
     * Function to create and send a elos event
     *
     * @param message: message to add to the event
     * @param memThreshold: memory threshold that was reached
     * @param cpuThreshold: cpu threshold that was reached
     * @param severity: severity of the event, defines action for punisher
     */
    void createElosEvent(const std::string &message, const unsigned long memThreshold, const double cpuThreshold,
                         elosSeverityE_t severity);

   public:
    /**
     * Construct a new Application object
     *
     * @param plugin: pointer of a plugin data structure
     * @param publisher: pointer to a publisher data structure
     * @param name: name of the application to monitor
     * @param thresholds: thresholds for the application
     * @param maxMemUsage: maximum memory usage for the application
     * @param maxCpuLoad: maximux cpu load for the application
     */
    Application(elosPlugin_t *plugin, elosPublisher *publisher, std::string name, elosThresholds thresholds,
                unsigned long maxMemUsage, double maxCpuLoad);

    /**
     * Function to check the usage of the application and calls createElosEvent
     */
    void checkUsages();
};

#endif
