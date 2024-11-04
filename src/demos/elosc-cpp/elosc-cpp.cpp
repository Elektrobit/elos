// SPDX-License-Identifier: MIT

#include <elos/libelos-cpp/libelos-cpp.h>

#include <iostream>
#include <string>

#include "safu/common.h"
#include "safu/log.h"

using namespace std;
using namespace elos;

int main() {
    elosResultE result = ELOS_RESULT_FAILED;
    Elos elosInst("127.0.0.1", 54323);

    result = elosInst.connect();

    if (result == ELOS_RESULT_OK) {
        safuLogInfo("connected to elosd");
        result = elosInst.disconnect();
        if (result == ELOS_RESULT_OK) {
            safuLogInfo("disconnected from elosd");
        } else {
            safuLogErr("Disconnect from elod failed");
        }
    } else {
        safuLogErr("Connect to elod failed");
    }

    return result;
}
