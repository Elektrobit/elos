// SPDX-License-Identifier: MIT
#include "elos/libelos-cpp/libelos-cpp.h"

#include <safu/log.h>
#include <safu/result.h>

#include <regex>
#include <stdexcept>
#include <string>

namespace elos {

elosUri elosParseUri(const std::string &uri) {
    elosUri newUri;
    std::string uriPattern =
        R"(^([a-zA-Z][a-zA-Z0-9+\-.]*):\/\/?([a-zA-Z0-9.-]+|\[?[0-9a-fA-F:.]+\]?)?(?::(\d{1,5}))?(\/[^\s?#]*)?(\?[^\s#]*)?(#[^\s]*)?$)";
    std::regex uriRegex(uriPattern);
    std::smatch matches;

    if (std::regex_match(uri, matches, uriRegex)) {
        newUri.scheme = matches[1].str();
        newUri.host = matches[2].str();
        newUri.port = matches[3].str();
        newUri.path = matches[4].str();
        newUri.query = matches[5].str();
        newUri.fragment = matches[6].str();
    } else {
        safuLogWarnF("Invalid URI format : %s", uri.c_str());
    }
    return newUri;
}

Event::Event() noexcept(false) {}

Event::Event(struct timespec date, elosEventSource_t &source, elosSeverityE_t severity, const std::string &hardwareid,
             uint64_t classification, elosEventMessageCodeE_t messageCode, const std::string &payload) noexcept(false) {
    elosResultE result = ELOS_RESULT_OK;

    event.date = date;
    result = (elosResultE)elosEventSourceDeepCopy(&event.source, &source);
    if (result == ELOS_RESULT_FAILED) {
        safuLogErr("copying event source failed!");
        throw ELOS_RESULT_FAILED;
    }
    event.severity = severity;
    event.messageCode = messageCode;
    event.classification = classification;
    event.hardwareid = strdup(hardwareid.c_str());
    if (!event.hardwareid) {
        safuLogErr("allocation for hardware ID failed!");
        throw ELOS_RESULT_FAILED;
    }
    event.payload = strdup(payload.c_str());
    if (!event.payload) {
        safuLogErr("allocation for payload failed!");
        throw ELOS_RESULT_FAILED;
    }
}

Event::~Event() noexcept(false) {
    elosResultE result = ELOS_RESULT_OK;
    result = (elosResultE)elosEventDeleteMembers(&event);
    if (result == ELOS_RESULT_FAILED) {
        safuLogErr("deleting event failed!");
        throw ELOS_RESULT_FAILED;
    }
}

Elos::Elos() : elosHost("127.0.0.1"), elosPort(54321), session{0, false} {}

Elos::Elos(const std::string &host, uint16_t port) : elosHost(host), elosPort(port), session{0, false} {}

Elos::Elos(const std::string &uri) {
    elosUri parsedUri = elosParseUri(uri);

    if (parsedUri.host.empty()) {
        safuLogWarn("Empty address, setting value to default");
        elosHost = "127.0.0.1";
    } else {
        elosHost = parsedUri.host;
    }

    try {
        elosPort = std::stoi(parsedUri.port);
    } catch (const std::invalid_argument &e) {
        safuLogWarn("Empty port, setting value to default");
        elosPort = 54321;
    }
    session = {0, false};
}

Elos::Elos(Elos &&other)
    : elosHost(std::move(other.elosHost)), elosPort(other.elosPort), session(std::move(other.session)) {}

Elos::~Elos() {
    if (elosSessionValid(&session)) {
        disconnect();
    }
}

elosResultE Elos::connect() noexcept {
    elosResultE result = ELOS_RESULT_OK;

    if (!elosSessionValid(&session)) {
        result = (elosResultE)elosConnectSessionTcpip(elosHost.c_str(), elosPort, &session);
    } else {
        safuLogErr("ElosCpp Connect from Tcpip failed: Connection already active");
        return ELOS_RESULT_FAILED;
    }

    if (result == ELOS_RESULT_FAILED) {
        safuLogErr("ElosCpp Connect to Tcpip failed!");
    }

    return result;
}

elosResultE Elos::disconnect() noexcept {
    elosResultE result = ELOS_RESULT_OK;

    if (elosSessionValid(&session)) {
        result = (elosResultE)elosDisconnectSession(&session);
    } else {
        safuLogErr("ElosCpp Disconnect from Tcpip failed: No active connection");
        return ELOS_RESULT_FAILED;
    }

    if (result == ELOS_RESULT_FAILED) {
        safuLogErr("ElosCpp Disconnect from Tcpip failed!");
    }

    return result;
}

elosResultE Elos::publish(UNUSED const elosEvent_t *event) {
    elosResultE result = ELOS_RESULT_OK;
    safuLogInfo("ElosCpp Publish");
    return result;
}

Subscription Elos::subscribe(std::string filter) {
    elosResultE result = ELOS_RESULT_OK;
    Subscription newSubscription;
    const char *filterArray[] = {filter.c_str(), nullptr};
    result = (elosResultE)elosEventSubscribe(&session, filterArray, 1, &newSubscription.subscription.eventQueueId);
    if (result != ELOS_RESULT_OK) {
        safuLogErr("Invalid Session, publish failed");
        newSubscription.subscription.eventQueueId = ELOS_ID_INVALID;
    }
    return newSubscription;
}
}  // namespace elos
