// SPDX-License-Identifier: MIT
#include <cstdint>
#include <memory>
#include <vector>

#include "constructor_utest.h"

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
    Elos defC;
    assert_string_equal(defC.elosHost.c_str(), "127.0.0.1");
    assert_int_equal(defC.elosPort, 54321);

    PARAM("Parameterize Constructor");
    Elos paramC(MOCK_IP_ADDR, MOCK_PORT);
    assert_string_equal(paramC.elosHost.c_str(), MOCK_IP_ADDR);
    assert_int_equal(paramC.elosPort, MOCK_PORT);

    std::vector<std::unique_ptr<Elos>> uriC(size);
    for (size_t i = 0; i < size; ++i) {
        PARAM("Constructor with uri string : %s", uriTestStrings[i].c_str());
        uriC[i] = std::make_unique<Elos>(uriTestStrings[i]);
        assert_string_equal(uriC[i]->elosHost.c_str(), uriTestHost[i].c_str());
        assert_int_equal(uriC[i]->elosPort, uriTestPort[i]);
    }

    PARAM("Move Constructor");
    Elos moveC(std::move(defC));
    assert_string_equal(moveC.elosHost.c_str(), "127.0.0.1");
    assert_int_equal(moveC.elosPort, 54321);
}
