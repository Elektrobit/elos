// SPDX-License-Identifier: MIT
#include <cstdint>
#include <memory>
#include <vector>

#include "constructor_utest.h"

namespace elos {

class ElosTest : public Elos {
   public:
    using Elos::Elos;

    const elosSession_t &getSession() const {
        return session;
    }

    uint16_t getPort() const {
        return elosPort;
    }

    const std::string &getHost() const {
        return elosHost;
    }
};

}  // namespace elos

int elosTestConstructorSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestConstructorSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestConstructorSuccess(UNUSED void **state) {
    using namespace elos;
    std::vector<std::string> uriTestStrings = {"https://www.test.com:8080/path/to/resource?query=1#fragment",
                                               "env://?envPrefix=UTEST_VARIABLE&testVar1=Val1&testVar2=Val2",
                                               "tcp://0.0.0.0:5555", ""};

    std::vector<std::string> uriTestHost = {"www.test.com", "127.0.0.1", "0.0.0.0", "127.0.0.1"};
    std::vector<std::uint64_t> uriTestPort = {8080, 54321, 5555, 54321};

    auto size = uriTestStrings.size();

    TEST("constructor");
    SHOULD("%s", "create successfully an elos instance");

    PARAM("Default Constructor");
    ElosTest defC;

    assert_string_equal(defC.getHost().c_str(), "127.0.0.1");
    assert_int_equal(defC.getPort(), 54321);

    PARAM("Parameterize Constructor");
    ElosTest paramC(MOCK_IP_ADDR, MOCK_PORT);
    assert_string_equal(paramC.getHost().c_str(), MOCK_IP_ADDR);
    assert_int_equal(paramC.getPort(), MOCK_PORT);

    for (size_t i = 0; i < size; ++i) {
        PARAM("Constructor with uri string : %s", uriTestStrings[i].c_str());
        auto uriC = std::make_unique<ElosTest>(uriTestStrings[i]);
        assert_string_equal(uriC->getHost().c_str(), uriTestHost[i].c_str());
        assert_int_equal(uriC->getPort(), uriTestPort[i]);
    }

    PARAM("Move Constructor");
    ElosTest moveC(std::move(defC));
    assert_string_equal(moveC.getHost().c_str(), "127.0.0.1");
    assert_int_equal(moveC.getPort(), 54321);

    PARAM("Default Destructor");
    try {
        auto defD = std::make_unique<Elos>();

        MOCK_FUNC_AFTER_CALL(elosConnectSessionTcpip, 0);
        expect_any(elosConnectSessionTcpip, host);
        expect_any(elosConnectSessionTcpip, port);
        expect_any(elosConnectSessionTcpip, session);
        will_return(elosConnectSessionTcpip, ELOS_RESULT_OK);

        defD->connect();

        MOCK_FUNC_AFTER_CALL(elosDisconnectSession, 0);
        expect_any(elosDisconnectSession, session);
        will_return(elosDisconnectSession, ELOS_RESULT_OK);

    } catch (const std::bad_alloc &e) {
        assert_true(false);
    } catch (safuResultE_t &ex) {
        assert_true(ex == SAFU_RESULT_OK);
    } catch (...) {
        assert_true(false);
    }
}
