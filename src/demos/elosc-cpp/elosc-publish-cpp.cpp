// SPDX-License-Identifier: MIT

#include <elos/libelos-cpp/libelos-cpp.h>

#include <iostream>
#include <string>

#include "safu/common.h"
#include "safu/log.h"

using namespace std;
using namespace elos;

void _printUsage(const char *const programName) {
    cout << "Usage: " << endl;
    cout << programName << "    default value" << endl;
    cout << programName << " -P <port>    set port" << endl;
    cout << programName << " -h    show usage" << endl;
}

int main(int argc, char *argv[]) {
    elosResultE result = ELOS_RESULT_FAILED;
    uint16_t port = 54321;

    if (argc > 1) {
        if (strcmp(argv[1], "-P") == 0) {
            if (argv[2] == NULL) {
                _printUsage(argv[0]);
                return 1;
            } else {
                port = std::stoi(argv[2]);
            }
        } else if (strcmp(argv[1], "-h") == 0) {
            _printUsage(argv[0]);
            return 0;
        }
    }

    Elos elosInst("127.0.0.1", port);

    Event myLogEvent;
    myLogEvent.event.payload = strdup("the publish API has many convenient ways to publish events");
    myLogEvent.event.messageCode = ELOS_MSG_CODE_DEBUG_LOG;

    Event eventInst({25, 100},
                    {strdup(std::string("appNameTest").c_str()), strdup(std::string("fileNameTest").c_str()), 0},
                    ELOS_SEVERITY_DEBUG, std::string("localhost").c_str(), 42, ELOS_MSG_CODE_NEW_SUBSCRIPTION,
                    std::string("payload").c_str());

    result = elosInst.publish(eventInst, true);
    if (result == ELOS_RESULT_OK) {
        safuLogInfo("event published to elosd");
    } else {
        safuLogErr("event publishing to elosd failed");
    }

    elosInst << myLogEvent;

    elosInst << Event{{1234567, 467},
                      {strdup("appName"), strdup("/bin/appFileName"), 42},
                      ELOS_SEVERITY_DEBUG,
                      "localhost",
                      42,
                      ELOS_MSG_CODE_NEW_SUBSCRIPTION,
                      "initializer list payload"};

    return result;
}
