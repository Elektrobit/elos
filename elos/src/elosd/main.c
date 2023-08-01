// SPDX-License-Identifier: MIT

#include <errno.h>
#include <locale.h>
#include <safu/common.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "elos/client_manager/client_manager.h"
#include "elos/config/config.h"
#include "elos/eventdispatcher/eventdispatcher.h"
#include "elos/eventlogging/LogAggregator.h"
#include "elos/eventprocessor/eventprocessor.h"
#include "elos/pluginmanager/pluginmanager.h"
#include "elos/scanner_manager/scanner_manager.h"
#include "safu/common.h"
#include "safu/log.h"
#include "samconf/samconf.h"
#include "version.h"

#define MAIN_SLEEP_TIME_USEC (100 * 1000)

struct serverContext {
    samconfConfig_t *config;
    elosClientManagerContext_t clientManagerContext;
    elosScannerManagerContext_t scannerManagerContext;
    elosPluginManager_t pluginManager;
    elosLogAggregator_t logAggregator;
    elosEventDispatcher_t eventDispatcher;
    elosEventProcessor_t eventProcessor;
};

static int elosActive = 1;

void elosSignalHandler(int signalno) {
    switch (signalno) {
        case SIGINT:
            safuLogDebug("Received SIGINT");
            elosActive = 0;
            signal(SIGINT, SIG_DFL);
            break;
        case SIGTERM:
            safuLogDebug("Received SIGTERM");
            signal(SIGTERM, SIG_DFL);
            elosActive = 0;
            break;
        default:
            safuLogWarn("Received unregistered signal");
            break;
    }
}

int elosServerShutdown(struct serverContext *ctx) {
    if (ctx == NULL) {
        safuLogErr("invalid parameter");
        return EXIT_FAILURE;
    }

    safuLogInfo("Shutting down...");
    int result = EXIT_SUCCESS;

    if (elosLogAggregatorShutdown(&ctx->logAggregator) != SAFU_RESULT_OK) {
        safuLogErr("Shutting down log aggregator failed!");
        result = EXIT_FAILURE;
    }
    if (elosClientManagerStop(&ctx->clientManagerContext) != 0) {
        safuLogErr("Stoping client manager failed!");
        result = EXIT_FAILURE;
    }
    if (elosScannerManagerStop(&ctx->scannerManagerContext) != NO_ERROR) {
        safuLogErr("Stoping scanner manager failed!");
        result = EXIT_FAILURE;
    }
    if (elosEventDispatcherDeleteMembers(&ctx->eventDispatcher) != SAFU_RESULT_OK) {
        safuLogErr("Deleting event dispatcher members failed!");
        result = EXIT_FAILURE;
    }
    if (elosEventProcessorDeleteMembers(&ctx->eventProcessor) != SAFU_RESULT_OK) {
        safuLogErr("Deleting event processor members failed!");
        result = EXIT_FAILURE;
    }
    if (elosPluginManagerDeleteMembers(&ctx->pluginManager) != SAFU_RESULT_OK) {
        safuLogErr("Deleting plugin processor members failed!");
        result = EXIT_FAILURE;
    }
    if (samconfConfigDelete(ctx->config) != SAMCONF_CONFIG_OK) {
        safuLogErr("Deleting config failed!");
        result = EXIT_FAILURE;
    }

    return result;
}

int main(int argc, char **argv) {
    int retval;
    struct serverContext context = {0};

    setlocale(LC_ALL, "C");
    safuLogSetPrefix(ELOSD_LOG_PREFIX);

    if (elosIsVersionRequested((const char **)argv, argc)) {
        const char *verstr;
        verstr = elosGetVersionString();
        safuLogInfoF("elosd-%s", verstr);
        return EXIT_SUCCESS;
    }

    if (signal(SIGINT, elosSignalHandler) == SIG_ERR) {
        safuLogErrErrno("signal_SIGINT");
        return EXIT_FAILURE;
    }

    if (signal(SIGTERM, elosSignalHandler) == SIG_ERR) {
        safuLogErrErrno("signal(SIGTERM)");
        return EXIT_FAILURE;
    }

    safuLogDebug("Load configuration");
    safuResultE_t result = elosConfigLoad(&context.config);
    if (result != SAFU_RESULT_OK) {
        safuLogErr("samconfLoad");
        elosServerShutdown(&context);
        return EXIT_FAILURE;
    }

    safuLogLevelE_t logLevel = elosConfigGetElosdLogLevel(context.config);
    safuLogStatusE_t loggingResult = safuLogSetStreamLevel(logLevel);
    if (loggingResult != SAFU_LOG_STATUS_SUCCESS) {
        safuLogWarnF("setting log level: %s failed! Fallback to log level: %s.", safuLogLevelToString(logLevel),
                     safuLogLevelToString(safuLogGetStreamLevel()));
    }
    loggingResult = safuLogSetFilterStr(elosConfigGetElosdLogFilter(context.config));
    if (loggingResult != SAFU_LOG_STATUS_SUCCESS) {
        safuLogWarn("setting log filter failed!");
    }

    safuLogInfoF("Setup:\n\tlisten on: %s:%d\n\thardwareid: %s\n\tlog level: %s\n\tlog filter: %s\n\tscanner path: %s",
                 elosConfigGetElosdInterface(context.config), elosConfigGetElosdPort(context.config),
                 safuGetHardwareId(), safuLogLevelToString(safuLogGetStreamLevel()),
                 elosConfigGetElosdLogFilter(context.config), elosConfigGetElosdScannerPath(context.config));

    safuLogDebug("Initialize PluginManager");
    elosPluginManagerParam_t const pmParam = {.config = context.config};
    result = elosPluginManagerInitialize(&context.pluginManager, &pmParam);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("elosPluginManagerInitialize had errors during execution");
    }

    safuLogDebug("Initialize LogAggregator");
    elosLogAggregatorParam_t const laParam = {.config = context.config, .pluginManager = &context.pluginManager};
    result = elosLogAggregatorStart(&context.logAggregator, &laParam);
    if (result != SAFU_RESULT_OK) {
        safuLogWarn("elosLogAggregatorStart had errors during execution");
    }

    safuLogDebug("Start EventProcessor");
    elosEventProcessorParam_t const epParam = {.config = context.config};
    retval = elosEventProcessorInitialize(&context.eventProcessor, &epParam);
    if (retval < 0) {
        safuLogErr("elosEventProcessorInitialize");
        elosServerShutdown(&context);
        return EXIT_FAILURE;
    }

    safuLogDebug("Start EventDispatcher");
    elosEventDispatcherParam_t const edParam = {
        .eventProcessor = &context.eventProcessor,
        .healthTimeInterval = NULL,
        .pollTimeout = NULL,
    };
    retval = elosEventDispatcherInitialize(&context.eventDispatcher, &edParam);
    if (retval < 0) {
        safuLogErr("elosEventDispatcherInitialize");
        elosServerShutdown(&context);
        return EXIT_FAILURE;
    } else {
        retval = elosEventDispatcherStart(&context.eventDispatcher);
        if (retval < 0) {
            safuLogErr("elosEventDispatcherStart");
            elosServerShutdown(&context);
            return EXIT_FAILURE;
        }
    }

    safuLogDebug("Start client manager");
    elosClientManagerParam_t cmParams = {
        .config = context.config,
        .logAggregator = &context.logAggregator,
        .eventProcessor = &context.eventProcessor,
        .eventDispatcher = &context.eventDispatcher,
    };
    retval = elosClientManagerStart(&context.clientManagerContext, &cmParams);
    if (retval < 0) {
        safuLogErr("elosClientManagerStart");
        elosServerShutdown(&context);
        return EXIT_FAILURE;
    }

    safuLogDebug("Start scanner manager");
    elosScannerManagerParam_t scannerManagerParam = {
        .config = context.config,
        .logAggregator = &context.logAggregator,
        .eventProcessor = &context.eventProcessor,
    };
    retval = elosScannerManagerStart(&context.scannerManagerContext, &scannerManagerParam);
    if ((retval != NO_ERROR) && (retval != NO_FATAL_ERRORS)) {
        safuLogErr("elosServerScannerLoad");
        elosServerShutdown(&context);
        return EXIT_FAILURE;
    }

    safuLogInfo("Running...");

    while (elosActive) {
        // Temporary solution to allow graceful program termination with raise() from threads
        usleep(MAIN_SLEEP_TIME_USEC);
    }

    retval = elosServerShutdown(&context);

    return retval;
}
