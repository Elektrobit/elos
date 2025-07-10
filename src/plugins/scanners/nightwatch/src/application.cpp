// SPDX-License-Identifier: MIT

#include "application.h"

#include <elos/event/event.h>
#include <safu/common.h>
#include <safu/log.h>
#include <unistd.h>

#include <chrono>
#include <fstream>
#include <memory>

Application::Application(elosPlugin_t *plugin, elosPublisher *publisher, std::string name, elosThresholds thresholds,
                         unsigned long maxMemUsage, double maxCpuLoad)
    : elosPlugin{plugin},
      elosPblisher{publisher},
      appName{std::move(name)},
      pid{},
      memUsageKb{0},
      cpuLoadPer{0},
      applicationThresholds{thresholds},
      maxMemoryUsage{maxMemUsage},
      maxCPULoad{maxCpuLoad} {}

std::string Application::execCmd(const char *cmd) {
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        safuLogErr("popen() failed!");
        return "";
    }

    std::array<char, 128> buffer;
    std::string result;
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

safuResultE_t Application::assignMemUsage() {
    unsigned int tSize = 0, resident = 0, share = 0;
    std::ifstream statmFile("/proc/" + pid + "/statm");
    if (!statmFile) {
        return SAFU_RESULT_FAILED;
    }
    statmFile >> tSize >> resident >> share;
    if (statmFile.fail()) {
        return SAFU_RESULT_FAILED;
    }
    statmFile.close();

    int pageSizeKb = getpagesize() / 1024;
    unsigned long rss = resident * pageSizeKb;
    unsigned long shared = share * pageSizeKb;
    memUsageKb = rss + shared;
    return SAFU_RESULT_OK;
}

safuResultE_t Application::assignCpuLoad() {
    std::string ignore;
    long uTime = 0, sTime = 0, startTime = 0;
    std::ifstream statFile("/proc/" + pid + "/stat");
    if (!statFile) {
        return SAFU_RESULT_FAILED;
    }
    // skip the first 13 fields for uTime (14) and sTime (15)
    for (int i = 0; i < 13; ++i) {
        statFile >> ignore;
        if (statFile.fail()) {
            return SAFU_RESULT_FAILED;
        }
    }
    // skip the next 5 fields to get startTime (22)
    statFile >> uTime >> sTime;
    if (statFile.fail()) {
        return SAFU_RESULT_FAILED;
    }
    for (int i = 0; i < 6; ++i) {
        statFile >> ignore;
        if (statFile.fail()) {
            return SAFU_RESULT_FAILED;
        }
    }
    statFile >> startTime;
    if (statFile.fail()) {
        return SAFU_RESULT_FAILED;
    }
    statFile.close();

    double systemUpTime;
    const double systemClkTck = sysconf(_SC_CLK_TCK);
    std::ifstream uptimeFile("/proc/uptime");
    if (!uptimeFile) {
        return SAFU_RESULT_FAILED;
    }
    uptimeFile >> systemUpTime;
    if (uptimeFile.fail()) {
        return SAFU_RESULT_FAILED;
    }
    uptimeFile.close();

    double uTimeSec = uTime / systemClkTck;
    double sTimeSec = sTime / systemClkTck;
    double startTimeSec = startTime / systemClkTck;

    double elapsedSec = systemUpTime - startTimeSec;
    double usageSec = uTimeSec + sTimeSec;
    cpuLoadPer = usageSec / elapsedSec;
    return SAFU_RESULT_OK;
}

void Application::checkUsages() {
    std::string pidCmd = "pgrep -n " + appName + " | tr -d '\\n'";
    pid = execCmd(pidCmd.c_str());
    if (pid.empty()) {
        safuLogWarnF("No Process ID found for process '%s'", appName.c_str());
        return;
    }
    if (assignMemUsage() == SAFU_RESULT_FAILED) {
        safuLogErrF("Could not assign mem usage of process '%s'", appName.c_str());
        return;
    }
    if (assignCpuLoad() == SAFU_RESULT_FAILED) {
        safuLogErrF("Could not assign cpu load of process '%s'", appName.c_str());
        return;
    }

    // check which threshold was reached
    if ((memUsageKb > maxMemoryUsage * applicationThresholds.memThreshold3) ||
        (cpuLoadPer > maxCPULoad * applicationThresholds.cpuThreshold3)) {
        createElosEvent("Threshold 3 was reached!", (maxMemoryUsage * applicationThresholds.memThreshold3),
                        (maxCPULoad * applicationThresholds.cpuThreshold3), ELOS_SEVERITY_FATAL);
    } else if ((memUsageKb > maxMemoryUsage * applicationThresholds.memThreshold2) ||
               (cpuLoadPer > maxCPULoad * applicationThresholds.cpuThreshold2)) {
        createElosEvent("Threshold 2 was reached!", (maxMemoryUsage * applicationThresholds.memThreshold2),
                        (maxCPULoad * applicationThresholds.cpuThreshold2), ELOS_SEVERITY_ERROR);
    } else if ((memUsageKb > maxMemoryUsage * applicationThresholds.memThreshold1) ||
               (cpuLoadPer > maxCPULoad * applicationThresholds.cpuThreshold1)) {
        createElosEvent("Threshold 1 was reached!", (maxMemoryUsage * applicationThresholds.memThreshold1),
                        (maxCPULoad * applicationThresholds.cpuThreshold1), ELOS_SEVERITY_WARN);
    } else {
        createElosEvent("Process is in Threshold", (maxMemoryUsage * applicationThresholds.memThreshold1),
                        (maxCPULoad * applicationThresholds.cpuThreshold1), ELOS_SEVERITY_INFO);
    }
}

void Application::createElosEvent(const std::string &message, const unsigned long memThreshold,
                                  const double cpuThreshold, elosSeverityE_t severity) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosEvent_t *event = new elosEvent_t;

    auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    struct timespec date;
    date.tv_sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    date.tv_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(duration - seconds).count();

    elosEventSource_t source;
    source.appName = strdup("nightwatch");
    source.fileName = strdup("nightwatch.so");
    source.pid = 0;

    std::string strPayload = "pid:" + pid + ";name:" + appName + ";memUsageKB:" + std::to_string(memUsageKb) +
                             ";cpuUsagePer:" + std::to_string(cpuLoadPer) +
                             ";memThreshold:" + std::to_string(memThreshold) +
                             ";cpuThreshold:" + std::to_string(cpuThreshold) + ";message:" + message;

    event->date = date;
    event->source = source;
    event->severity = severity;
    event->hardwareid = strdup(safuGetHardwareId());
    event->messageCode = ELOS_MSG_CODE_RESOURCE_OVERLOAD;
    event->payload = strdup(strPayload.c_str());

    result = elosPluginPublish(elosPlugin, elosPblisher, event);
    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("Failed to publish event");
    } else if (result == SAFU_RESULT_OK) {
        safuLogDebugF("Event for process '%s' ('%s') was published", appName.c_str(), pid.c_str());
    }

    result = elosEventDeleteMembers(event);
    if (result == SAFU_RESULT_FAILED) {
        safuLogErr("Failed to delete members of event");
    } else {
        delete event;
    }
}
