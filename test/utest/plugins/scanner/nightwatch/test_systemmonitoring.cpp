#include <gtest/gtest.h>

#include <condition_variable>
#include <mutex>

#include "night_watch_test.h"
#include "systemMonitoring.h"

class SystemMonitoringTest : public NightWatchTest {
   public:
    void SetUp() override {
        NightWatchTest::SetUp();

        createConfigFileWithRunningProcess();
    }

    void TearDown() {
        NightWatchTest::TearDown();

        std::remove(confPath);
    }

    void createConfigFileWithRunningProcess() {
        std::string conf = R"(
        systemmonitoring:
        # Overwrites default values if set here
            testinterval: 20 # optional
            memthreshold1: 1.4 # default is 1.5 (optional)
            memthreshold2: 1.5 # default is 1.7 (optional)
            memthreshold3: 1.6 # default is 2.0 (optional)
            cputhreshold1: 2.4 # default is 1.25 (optional)
            cputhreshold2: 2.5 # default is 2.0 (optional)
            cputhreshold3: 2.6 # default is 3.0 (optional)
            monitoring:
                monitoring1:
                    testinterval: 3000 # optional
                    memthreshold1: 1.4 # default is 1.5 (optional)
                    memthreshold2: 1.5 # default is 1.7 (optional)
                    memthreshold3: 1.6 # default is 2.0 (optional)
                    cputhreshold1: 2.4 # default is 1.25 (optional)
                    cputhreshold2: 2.5 # default is 2.0 (optional)
                    cputhreshold3: 2.6 # default is 3.0 (optional)
                    applications:
                        init:
                            memthreshold1: 1.3 # default is 1.5 (optional)
                            maxmemusage: 10 # needed in KB

                            # Erweiterung des Plugins
                            cputhreshold1: 2.7 # default is 1.25 (optional)
                            cputhreshold2: 2.8 # default is 2.0 (optional)
                            cputhreshold3: 2.9 # default is 3.0 (optional)
                            maxcpuload: 10 # needed in %
        )";

        std::ofstream os{confPath};
        os << conf;
    }

    const char *confPath = "/tmp/nightwatch_conf.yaml";
};

// Test that SystemMonitoring logs an error if the configuration file is not found
TEST_F(SystemMonitoringTest, config_file_not_found) {
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        const auto expectedErrorMessage = "Failed to load file: '%s'";

        expectedMessageFound |= (logMessage.find(expectedErrorMessage) != std::string::npos);

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Create SystemMonitoring using configuration file path that doesn't exist
    SystemMonitoring monitor{plugin, nullptr, "NOT FOUND"};

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}

// Test that calling runMonitoringThread tries to publish an event
TEST_F(SystemMonitoringTest, run_and_stop) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        const auto expectedErrorMessage = "Event for process '%s' ('%s') was published";

        {
            std::lock_guard<std::mutex> lock{mutex};
            expectedMessageFound |= (logMessage.find(expectedErrorMessage) != std::string::npos);

            if (expectedMessageFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Create SystemMonitoring using an existing configuration file
    SystemMonitoring monitor{plugin, nullptr, confPath};

    // Call runMonitoringThread()
    monitor.runMonitoringThread();

    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return expectedMessageFound; });
    }

    // Call stopMonitoringThread()
    monitor.stopMonitoringThread();

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}
