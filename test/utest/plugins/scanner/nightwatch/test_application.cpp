#include "application.h"
#include "night_watch_test.h"

class ApplicationTest : public NightWatchTest {
   public:
    void SetUp() override {
        NightWatchTest::SetUp();

        safuLogSetCallbackFunc(nullptr);

        plugin->publish = [](struct elosPublisher *publisher, const elosEvent_t *event) {
            const auto eventMessage = event->payload;

            correctMessageIsFound = messageEndsWith(eventMessage, expectedMessage);

            return SAFU_RESULT_OK;
        };
    }

    static std::string expectedMessage;
    static bool correctMessageIsFound;

   private:
    static bool messageEndsWith(const std::string &fullMsg, const std::string &str) {
        auto pos = fullMsg.find(str);
        auto size = str.size();
        auto messageSize = fullMsg.size();

        return messageSize == size + pos;
    };
};

std::string ApplicationTest::expectedMessage = "";
bool ApplicationTest::correctMessageIsFound = false;

// Test that the plugin publishes a log message specifying that a the PID is not found for the given process
TEST_F(ApplicationTest, empty_pid) {
    expectedMessage = "No Process ID found for process '%s'";

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) -> safuLogStatusE {
        const std::string &logMessage = msg.fmt;

        correctMessageIsFound = logMessage.find(expectedMessage) == 0;

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Create Application using a non-existing process
    Application application{plugin, nullptr, "non-existing-process", {}, 0, 0};

    // Call checkUsages()
    application.checkUsages();

    // Verify that expected log message is found
    EXPECT_TRUE(correctMessageIsFound);
}

// Expect that checkUsages() causes the plugin to publish an event specifying that threshold 3 is reached
// if (memUsage > maxMemUsage * memThreshold3)
TEST_F(ApplicationTest, reach_threshold_3) {
    expectedMessage = ";message:Threshold 3 was reached!";

    constexpr auto existing_process = "init";
    // Create Application using an existing process "init"
    Application application{plugin, nullptr, existing_process, {}, 0, 0};

    // Call checkUsages()
    application.checkUsages();

    // Verify that expected log message is found
    EXPECT_TRUE(correctMessageIsFound);
}

// Expect that checkUsages() causes the plugin to publish an event specifying that threshold 2 is reached
// if (memUsage > maxMemUsage * memThreshold2)
TEST_F(ApplicationTest, reach_threshold_2) {
    Thresholds thresholds{
        .memThreshold3 = 0xFFFFFF,
    };

    expectedMessage = ";message:Threshold 2 was reached!";

    constexpr auto existing_process = "init";
    // Create Application using an existing process "init" and memThreshold3 = 0xFFFFFF
    Application application{plugin, nullptr, existing_process, thresholds, 100, 0};

    // Call checkUsages()
    application.checkUsages();

    // Verify that checkUsages() causes the plugin to publish an event with threshold 2 is reached
    EXPECT_TRUE(correctMessageIsFound);
}

// Expect that checkUsages() causes the plugin to publish an event specifying that threshold 1 is reached
// if (memUsage > maxMemUsage * memThreshold1)
TEST_F(ApplicationTest, reach_threshold_1) {
    Thresholds thresholds{
        .memThreshold2 = 0xFFFFFF,
        .memThreshold3 = 0xFFFFFF,
    };

    expectedMessage = ";message:Threshold 1 was reached!";

    constexpr auto existing_process = "init";
    // Create Application using an existing process "init" and thresholds
    // memThreshold2 = 0xFFFFFF , memThreshold3 = 0xFFFFFF
    Application application{plugin, nullptr, existing_process, thresholds, 100, 0};

    // Call checkUsages()
    application.checkUsages();

    // Verify that expected log message is found
    EXPECT_TRUE(correctMessageIsFound);
}

// Expect that checkUsages() causes the plugin to publish an event specifying that the process
// is withn thresholdT
TEST_F(ApplicationTest, within_thresholds) {
    Thresholds thresholds{
        .memThreshold1 = 0xFFFFFF,
        .memThreshold2 = 0xFFFFFF,
        .memThreshold3 = 0xFFFFFF,
    };

    expectedMessage = "Process is in Threshold";

    constexpr auto existing_process = "init";
    // Create Application using an existing process "init" and thresholds
    // memThreshold1 = 0xFFFFFF , memThreshold2 = 0xFFFFFF, memThreshold3 = 0xFFFFFF
    Application application{plugin, nullptr, existing_process, thresholds, 100, 0};

    // Call checkUsages()
    application.checkUsages();

    // Verify that expected log message is found
    EXPECT_TRUE(correctMessageIsFound);
}
