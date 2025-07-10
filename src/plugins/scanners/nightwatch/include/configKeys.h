// SPDX-License-Identifier: MIT

#ifndef CONFIGKEYS_H
#define CONFIGKEYS_H

#include <string>

struct elosConfigKeys {
    std::string systemmonitoring = "systemmonitoring";
    std::string monitoring = "monitoring";
    std::string apps = "applications";
    std::string testinterval = "testinterval";
    std::string memthreshold1 = "memthreshold1";
    std::string memthreshold2 = "memthreshold2";
    std::string memthreshold3 = "memthreshold3";
    std::string cputhreshold1 = "cputhreshold1";
    std::string cputhreshold2 = "cputhreshold2";
    std::string cputhreshold3 = "cputhreshold3";
    std::string maxmemusage = "maxmemusage";
    std::string maxcpuload = "maxcpuload";
};

#endif
