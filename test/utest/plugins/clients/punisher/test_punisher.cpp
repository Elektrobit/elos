#include <gtest/gtest.h>
#include <safu/log.h>

#include <condition_variable>

#include "punisher.h"

class PunisherTest : public ::testing::Test {
   public:
    void SetUp() override {
        eventPayload = "pid:1234;name:hello;";

        plugin->createSubscriber = [](auto, auto) { return SAFU_RESULT_OK; };

        plugin->subscribe = [](auto, auto, auto, auto) { return SAFU_RESULT_OK; };

        plugin->unsubscribeAll = [](auto) { return SAFU_RESULT_OK; };

        plugin->readQueue = [](auto, auto, safuVec_t **events) {
            *events = (safuVec_t *)malloc(sizeof(safuVec_t));

            *events[0] = safuVec{
                sizeof(elosEvent_t),
                sizeof(elosEvent_t),
                1,
                createEvent(),
            };

            return SAFU_RESULT_OK;
        };

        safuResultE_t subscription = elosPluginCreateSubscriber(plugin, &subscriber);
        ASSERT_EQ(subscription, SAFU_RESULT_OK);
    }

    virtual void TearDown() override {
        delete plugin;
    }

    elosPlugin_t *plugin = new elosPlugin_t;
    elosSubscriber *subscriber;

    static std::string eventPayload;
    static elosSeverityE_t severity;
    static elosEventMessageCodeE_t messageCode;

    static elosEvent *createEvent() {
        auto eventSource = std::make_unique<elosEventSource_t>(elosEventSource_t{
            (char *)malloc(sizeof(char)),
            (char *)malloc(sizeof(char)),
            1234,
        });

        char *payload = (char *)malloc(eventPayload.size() + 1);
        std::copy(eventPayload.begin(), eventPayload.end() + 1, payload);

        elosEvent *event = (elosEvent_t *)malloc(sizeof(elosEvent_t));
        *event = elosEvent_t{
            {}, std::move(*eventSource), severity, (char *)malloc(sizeof(char)), 0, messageCode, payload,
        };

        return event;
    }
};

std::string PunisherTest::eventPayload = "pid:1234;name:hello;";
elosSeverityE_t PunisherTest::severity = ELOS_SEVERITY_INFO;
elosEventMessageCodeE_t PunisherTest::messageCode = ELOS_MSG_CODE_RESOURCE_OVERLOAD;

// Test that subscription fails during constrcution if the elos plugin subscription fails
TEST_F(PunisherTest, construction_subscription_failure) {
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        expectedMessageFound |= (logMessage == "Could not subscribe to event filter: '%s'");

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Make the plugin fail any subscription
    plugin->subscribe = [](auto, auto, auto, auto) { return SAFU_RESULT_FAILED; };

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Verity that a subscription failure message is logged
    EXPECT_TRUE(expectedMessageFound);
}

// Test that Punisher fails to unsubscribe during destruction if
// plugin unsubscribeAll fails
TEST_F(PunisherTest, destructor_unsubscription_failure) {
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        expectedMessageFound |= (logMessage == "Could not unsubscribe the subscribtion");

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Make the plugin fail unsubscription
    plugin->unsubscribeAll = [](auto) { return SAFU_RESULT_FAILED; };

    // Construct Punisher
    auto punisher = std::make_unique<Punisher>(plugin, subscriber);

    // Destruct Punisher
    punisher = nullptr;

    // Verity that a unsubscription failure message is logged
    EXPECT_TRUE(expectedMessageFound);
}

// Test that a process can run within the expected resources threshold
TEST_F(PunisherTest, process_within_threshold) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            expectedMessageFound |= (logMessage == "Process '%s' is in Threshold. No action taken");

            if (expectedMessageFound) {
                cv.notify_one();
            }
        }
        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Set the event severity to ELOS_SEVERITY_INFO
    severity = ELOS_SEVERITY_INFO;

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return expectedMessageFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}

// Test that a process can run within the first threshold
TEST_F(PunisherTest, reach_threshold_n_1) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            expectedMessageFound |= (logMessage == "Process '%s' reached Threshold 1. No action taken");

            if (expectedMessageFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Set the event severity to ELOS_SEVERITY_WARN
    severity = ELOS_SEVERITY_WARN;

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return expectedMessageFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}

// Test that a process gets restarted if threshold 2 is reached
TEST_F(PunisherTest, reach_threshold_n_2) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            expectedMessageFound |= (logMessage == "Process '%s' reached Threshold 2. Restart process");

            if (expectedMessageFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Set the event severity to ELOS_SEVERITY_ERROR
    severity = ELOS_SEVERITY_ERROR;

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return expectedMessageFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}

// Test that a process gets killed if threshold 3 is reached
TEST_F(PunisherTest, reach_threshold_n_3) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            expectedMessageFound |= (logMessage == "Process '%s' reached Threshold 3. Kill process");

            if (expectedMessageFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Set the event severity to ELOS_SEVERITY_FATAL
    severity = ELOS_SEVERITY_FATAL;

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return expectedMessageFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}

// Test that an error gets logged if a process can't be found by name
TEST_F(PunisherTest, process_name_not_found) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool processNameNotFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            processNameNotFound |= (logMessage == "Could not find process name");

            if (processNameNotFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Set invalid event payload which has only pid but no name
    eventPayload = "pid:1234;";

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return processNameNotFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(processNameNotFound);
}

// Test that an error gets logged if a process can't be found by PID
TEST_F(PunisherTest, process_pid_not_found) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool processPidNotFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) -> safuLogStatusE_t {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            processPidNotFound |= (logMessage == "Could not find process id");

            if (processPidNotFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Set invalid event payload which has neither pid nor name
    eventPayload = "INVALID PID CONTENT";

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return processPidNotFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(processPidNotFound);
}

// Test that an error gets logged if an event can't be read from the queue
TEST_F(PunisherTest, cannot_read_event) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) -> safuLogStatusE_t {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            expectedMessageFound |= (logMessage == "Could not read event queue");

            if (expectedMessageFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Make reading events from the queue fail
    plugin->readQueue = [](auto, auto, auto) { return SAFU_RESULT_FAILED; };

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Set the event severity to ELOS_SEVERITY_FATAL
    severity = ELOS_SEVERITY_FATAL;

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return expectedMessageFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}

// Test that event won't be handled if its message code is not ELOS_MSG_CODE_RESOURCE_OVERLOAD
TEST_F(PunisherTest, event_does_not_match_format) {
    static std::mutex mutex;
    static std::condition_variable cv;
    static bool expectedMessageFound = false;

    // Set the logging callback
    safuLogSetCallbackFunc([](auto msg) -> safuLogStatusE_t {
        const std::string &logMessage = msg.fmt;

        {
            std::lock_guard<std::mutex> lock{mutex};
            expectedMessageFound |= (logMessage == "Event does not match format");

            if (expectedMessageFound) {
                cv.notify_one();
            }
        }

        return SAFU_LOG_STATUS_SUCCESS;
    });

    // Set the message code of the event to ELOS_MSG_CODE_READ_ERROR
    messageCode = ELOS_MSG_CODE_READ_ERROR;

    // Construct Punisher
    Punisher punisher{plugin, subscriber};

    // Call runEventHandlingThread()
    punisher.runEventHandlingThread();

    // Wait until the message is logged
    {
        std::unique_lock<std::mutex> lock{mutex};
        cv.wait(lock, []() { return expectedMessageFound; });
    }

    // Call stopEventHandlingThread()
    punisher.stopEventHandlingThread();

    // Verify that expected log message is found
    EXPECT_TRUE(expectedMessageFound);
}
