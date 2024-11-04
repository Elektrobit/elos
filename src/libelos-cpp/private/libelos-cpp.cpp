// SPDX-License-Identifier: MIT
#include "elos/libelos-cpp/libelos-cpp.h"

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
    session = {0, false};
}

safuResultE_t Elos::connect() noexcept {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Connect to Tcpip");
    return result;
}

safuResultE_t Elos::disconnect() noexcept {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Disonnect");
    return result;
}

safuResultE_t Elos::publish(UNUSED const elosEvent_t *event) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Publish");
    return result;
}

safuResultE_t Elos::subscribe(UNUSED const char **filterRuleArray, UNUSED size_t filterRuleArraySize,
                              UNUSED elosEventQueueId_t *eventQueueId) {
    safuResultE_t result = SAFU_RESULT_OK;
    safuLogInfo("ElosCpp Subscribe");
    return result;
}
}  // namespace elos
