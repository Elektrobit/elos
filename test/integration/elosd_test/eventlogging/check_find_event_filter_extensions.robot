*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check find event filter extensions in elosd

Library             DataDriver    file=../../test_data/check_find_filter.csv    dialect=unix
Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource
Library             ../../libraries/ElosKeywords.py

Test Template       Check Given Find Event Filter With Error Expectation


*** Variables ***
${ELOSD_VERSION}            1
${ELOSD_PUBLISH_CODE}       2
${ELOSD_QUERY_CODE}         4
${ELOSD_PUBLISH_MESSAGE}    {"messageCode": 4,"payload":"message from tcp published"}


*** Test Cases ***
Check Given Find Event Filter ${event_filter} With Error Expectation ${is_error_expected}


*** Keywords ***
Check Given Find Event Filter With Error Expectation
    [Documentation]    Template to check different types of
    ...    event filters.
    [Arguments]    ${event_filter}    ${is_error_expected}

    Connect To Elosd
    Send Message To Elosd    ${ELOSD_VERSION}    ${ELOSD_PUBLISH_CODE}    ${ELOSD_PUBLISH_MESSAGE}
    No Error Received
    Check Event Filter
    ...    ${ELOSD_VERSION}
    ...    ${ELOSD_QUERY_CODE}
    ...    ${event_filter}
    ...    ${is_error_expected}
    Disconnect From Elosd

No Error Received
    [Documentation]    Message received from elosd is not an error

    ${message}=    Receive Message From Elosd
    Should Contain    ${message}    "error":null

Check Event Filter
    [Documentation]    Use given query to retrieve published event from    elosd Target
    [Arguments]    ${version}    ${messagecode}    ${message}    ${error_expected}

    Send Message To Elosd    ${version}    ${messagecode}    ${message}
    ${response}=    Receive Message From Elosd
    Log    ${response}
    IF    '${error_expected}' == 'YES'
        Should Not Contain    ${response}    message from tcp published
    ELSE
        Should Contain    ${response}    message from tcp published
    END
