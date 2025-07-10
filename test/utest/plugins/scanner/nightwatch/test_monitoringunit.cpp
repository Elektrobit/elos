#include <gtest/gtest.h>
#include <safu/log.h>

#include "monitoringUnit.h"
#include "night_watch_test.h"

class MonitoringUnitTest : public NightWatchTest {};

// Test missing keys maxmemusage and maxcpuload
TEST_F(MonitoringUnitTest, missing_maxmemusage_and_maxcpuload) {
    static int messageCount = 0;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        const auto expectedErrorMessage =
            "Key in config of application '%s' not found: '%s'. Need to be added to the config!.";
        messageCount += (logMessage.find(expectedErrorMessage) != std::string::npos);

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Create MonitoringUnit using empty thresholds
    MonitoringUnit monitoringUnit{1, elosThresholds{}};

    // Create an appsData yaml config object with one app but without the keys maxmemusage and maxcpuload
    YAML::Node appsData;
    appsData["app"] = YAML::Node{};

    // Call addApps() using appsData
    monitoringUnit.addApps(appsData, plugin, nullptr);

    // Verify that two error messages are logged for the two missing keys
    EXPECT_EQ(messageCount, 2);
}

// Test that an error gets logged when trying to add a null appsData config object
TEST_F(MonitoringUnitTest, add_null_app_data) {
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        const auto expectedErrorMessage =
            "No app data found. Check if the config file is missing data of app in the monitoring unit!";

        expectedMessageFound |= (logMessage.find(expectedErrorMessage) != std::string::npos);

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Create MonitoringUnit using empty thresholds
    MonitoringUnit monitoringUnit{10, elosThresholds{}};

    // Create an appsData yaml config object without any keys
    YAML::Node appsData;

    // Call addApps() using appsData
    monitoringUnit.addApps(appsData, plugin, nullptr);

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}

// Test that runCheckingOfAppsUsages() for an existing process with a valid config doesn't log errors
TEST_F(MonitoringUnitTest, add_app_for_existing_process) {
    static bool processIsMissing = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        const auto expectedErrorMessage = "No Process ID found for process '%s'";

        bool missingProcessLogFound = logMessage.find(expectedErrorMessage) == 0;
        processIsMissing |= missingProcessLogFound;

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Create MonitoringUnit using empty thresholds
    MonitoringUnit monitoringUnit{10, elosThresholds{}};

    YAML::Node app;
    app["memthreshold1"] = 100;
    app["cputhreshold1"] = 100;
    app["maxmemusage"] = 100;
    app["maxcpuload"] = 100;

    // Create appsData conf object that contains an existing process with valid configs
    constexpr auto existingProcess = "init";
    YAML::Node appsData;
    appsData[existingProcess] = app;

    // Call addApps() using appsData
    monitoringUnit.addApps(appsData, plugin, nullptr);

    // Call runCheckingOfAppsUsages()
    monitoringUnit.runCheckingOfAppsUsages();

    // Verify that the prcess exists and no error logs are printed
    EXPECT_FALSE(processIsMissing);
}

// Test that getTestInterval() returns the correct value
TEST_F(MonitoringUnitTest, get_test_interval) {
    // Create MonitoringUnit
    MonitoringUnit monitoringUnit{10, elosThresholds{}};

    // Call getTestInterval()
    // Verify that calling getTestInterval() returns the correct value
    EXPECT_EQ(monitoringUnit.getTestInterval(), 10);
}
