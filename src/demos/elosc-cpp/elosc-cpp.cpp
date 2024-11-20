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

    result = elosInst.connect();
    if (result == ELOS_RESULT_OK) {
        safuLogInfo("connected to elosd");
        result = elosInst.disconnect();
        if (result == ELOS_RESULT_OK) {
            safuLogInfo("disconnected from elosd");
        } else {
            safuLogErr("Disconnect from elosd failed");
        }

    } else {
        safuLogErr("Connect to elod failed");
    }

    return result;
}
