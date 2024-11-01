// SPDX-License-Identifier: MIT

#include <safu/common.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "elos/event/event.h"
#include "elos/libelos/libelos.h"
#include "version.h"

// https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define catch_errors(retval) catch_errors_func(__func__, __LINE__, retval)
static void catch_errors_func(const char *func, int line, int retval) {
    if (retval < 0) {
        fprintf(stderr, "Error: retval < 0 (%d); %s, %s in %d.\n", retval, __FILENAME__, func, line);
        exit(1);
    }
}

static int elosRun = 1;
void elosSigintHandler(UNUSED int unused) {
    elosRun = 0;
}

int main(int argc, char **argv) {
    elosSession_t *session;
    safuResultE_t result;
    const char *version;
    const char *eventFilterStringA[] = {".event.hardwareid 'abc' STRCMP", ".event.messageCode 8000 EQ"};
    const char *eventFilterStringB[] = {".event.severity 4 EQ"};
    elosEventQueueId_t eventQueueIdA = ELOS_ID_INVALID;
    elosEventQueueId_t eventQueueIdB = ELOS_ID_INVALID;
    elosEventVector_t *eventVector = NULL;
    elosEvent_t *event;
    elosEvent_t eventA = {
        .hardwareid = "abc", .messageCode = ELOS_MSG_CODE_WILDCARD_8XXX, .severity = ELOS_SEVERITY_ERROR};
    elosEvent_t eventB = {
        .hardwareid = "123", .messageCode = ELOS_MSG_CODE_WILDCARD_8XXX, .severity = ELOS_SEVERITY_WARN};
    elosEvent_t eventC = {.hardwareid = "234", .messageCode = ELOS_MSG_CODE_USER_ADDED, .severity = ELOS_SEVERITY_INFO};

    if (elosIsVersionRequested((const char **)argv, argc)) {
        elosPrintVersion(stdout, "libelos_demo");
        fprintf(stdout, "\n");
        return 0;
    }

    signal(SIGINT, elosSigintHandler);

    printf("connecting to event logging and management system...\n");
    result = elosConnectTcpip("127.0.0.1", strtoul(safuGetEnvOr("ELOSD_PORT", "54321"), NULL, 10), &session);
    catch_errors(result);

    printf("get event logging and management system version...\n");
    result = elosGetVersion(session, &version);
    catch_errors(result);
    printf("  event logging and management system is version '%s'\n", version);

    result = elosEventSubscribe(session, eventFilterStringA, ARRAY_SIZE(eventFilterStringA), &eventQueueIdA);
    catch_errors(result);

    result = elosEventSubscribe(session, eventFilterStringB, ARRAY_SIZE(eventFilterStringB), &eventQueueIdB);
    catch_errors(result);

    result = elosEventPublish(session, &eventA);
    catch_errors(result);

    result = elosEventPublish(session, &eventB);
    catch_errors(result);

    result = elosEventPublish(session, &eventC);
    catch_errors(result);

    result = elosEventPublish(session, &eventC);
    catch_errors(result);

    result = elosEventQueueRead(session, eventQueueIdB, &eventVector);
    catch_errors(result);

    event = safuVecGetLast(eventVector);
    if (event != NULL) {
        printf("event: %d, %s\n", event->severity, event->hardwareid);
    }

    printf("disconnecting from event logging and management system...\n");
    result = elosDisconnect(session);
    catch_errors(result);
    return 0;
}
