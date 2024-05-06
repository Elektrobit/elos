*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check mix of valid and invalid authorized process filter

Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource
Library             ../../libraries/TemplateConfig.py
Library             ../../libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${BLACKLIST_FILTER}                 .event.messageCode 2010 EQ
@{AUTHORIZED_PROCESS_FILTERS}
...                                 .process.exec '${ELOSC_PATH}' STRCMP
...                                 .process.uid 200 EQ
...                                 .process.gid 0 EQ


*** Test Cases ***
01_Test_Valid_And_Invalid_Filters    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    A process should match at least one filter to
    ...    be a authorized process

    Given Valid And Invalid Authorized Process Filters Are Set
    When Client Tries To Publish A Blacklisted Event
    Then Blacklisted Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
Valid And Invalid Authorized Process Filters Are Set
    [Documentation]    Set valid and invalid authorized process filter in config

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template
    ...    EventBlacklist=${BLACKLIST_FILTER}
    ...    authorizedProcesses=${AUTHORIZED_PROCESS_FILTERS}
    Start Elosd
    Wait Till Elosd Is Started

Client Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${PUBLISH_TIME}    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    ${rc}    Execute And Log Based On User Permissions
    ...    elosc -p '{"messageCode": 2010}'
    ...    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter

Blacklisted Event Is Published
    [Documentation]    Blacklisted event will be published from authorized clients

    ${stdout}    ${rc}    Execute And Log
    ...    elosc -f ".event.messageCode 2010 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND"
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter

Reset Elosd Config
    [Documentation]    reset elosd config to default during test teardown.

    Stop Elosd
    Wait For Elosd To Stop
    Cleanup Template Config
    Start Elosd
    Wait Till Elosd Is Started
