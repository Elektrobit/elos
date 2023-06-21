// SPDX-License-Identifier: MIT

#include <samconf/uri.h>
#include <string.h>

#include "cmocka_mocks/mock_libc.h"
#include "mock_samconf.h"
#include "samconfUriInit_utest.h"

int samconfTestSamconfUriInitSuccessSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfUriInitSuccessTeardown(UNUSED void **state) {
    return 0;
}

static void samconfTestSamconfUriInitSuccessParam(const char *uriString, const char *scheme, const char *authority,
                                                  const char *path, const char *query, const char *fragment) {
    samconfConfigStatusE_t status;
    samconfUri_t uri;

    memset(&uri, 0xFF, sizeof(uri));

    status = samconfUriInit(&uri, uriString);

    assert_string_equal(uri.plain, uriString);
    assert_string_equal(uri.scheme, scheme);
    assert_string_equal(uri.authority, authority);
    assert_string_equal(uri.path, path);
    assert_string_equal(uri.query, query);
    assert_string_equal(uri.fragment, fragment);
    assert_int_equal(status, SAMCONF_CONFIG_OK);

    samconfUriDeleteMembers(&uri);
}

void samconfTestSamconfUriInitSuccess(UNUSED void **state) {
    TEST("samconfUriInit");
    SHOULD("%s", "initialize configuration signature uri");

    samconfTestSamconfUriInitSuccessParam("ftp://ftp.is.co.za/rfc/rfc1808.txt", "ftp", "ftp.is.co.za",
                                          "/rfc/rfc1808.txt", "", "");
    samconfTestSamconfUriInitSuccessParam("http://www.ietf.org/rfc/rfc2396.txt", "http", "www.ietf.org",
                                          "/rfc/rfc2396.txt", "", "");
    samconfTestSamconfUriInitSuccessParam("ldap://[2001:db8::7]/c=GB?objectClass?one", "ldap", "[2001:db8::7]", "/c=GB",
                                          "?objectClass?one", "");
    samconfTestSamconfUriInitSuccessParam("mailto:John.Doe@example.com", "mailto", "", "John.Doe@example.com", "", "");
    samconfTestSamconfUriInitSuccessParam("news:comp.infosystems.www.servers.unix", "news", "",
                                          "comp.infosystems.www.servers.unix", "", "");
    samconfTestSamconfUriInitSuccessParam("telnet://192.0.2.16:80/", "telnet", "192.0.2.16:80", "/", "", "");
}
