#ifndef NIGHT_WATCH_TEST
#define NIGHT_WATCH_TEST

#include <elos/libelosplugin/libelosplugin.h>
#include <gtest/gtest.h>

class NightWatchTest : public ::testing::Test {
   public:
    void SetUp() override {
        plugin->createSubscriber = [](auto, auto) { return SAFU_RESULT_OK; };

        plugin->publish = [](auto, auto) { return SAFU_RESULT_OK; };
    }

    virtual void TearDown() override {
        delete plugin;
    }

    elosPlugin_t *plugin = new elosPlugin_t;
};

#endif
