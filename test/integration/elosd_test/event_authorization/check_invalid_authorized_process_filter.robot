*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check invalid authorized process filter

Resource            ../../elosd-keywords.resource
Resource            ../../keywords.resource
Library             ../../libraries/TemplateConfig.py

Suite Setup         Connect To Target And Log In
Suite Teardown      Close All Connections


*** Variables ***
${BLACKLIST_FILTER}             .event.messageCode 2010 EQ
@{INVALID_PROCESS_FILTERS}      42    jksdflh    myfilter_32    @34!!


*** Test Cases ***
01_Test_Invalid_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Invalid process filter renders a process unauthorized

    Given An Invalid Authorized Process Filter Is Set
    When Client Tries To Publish A Blacklisted Event
    Then A Security Event Is Published
    [Teardown]    Reset Elosd Config

02_Test_Invalid_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Normal events are published though process filter is invalid

    Given An Invalid Authorized Process Filter Is Set
    When Client Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
An Invalid Authorized Process Filter Is Set
    [Documentation]    Set an invalid authorized process filter in config

    Stop Elosd
    Wait For Elosd To Stop
    Set Config From Template
    ...    EventBlacklist=${BLACKLIST_FILTER}
    ...    authorizedProcesses=${INVALID_PROCESS_FILTERS}
    Start Elosd
    Wait Till Elosd Is Started

Client Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${PUBLISH_TIME}    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    ${rc}    Execute And Log Based On User Permissions
    ...    elosc -p '{"messageCode": 2010}'
    ...    ${RETURN_RC}
    Executable Returns An Error    ${rc}

Client Tries To Publish A Normal Event
    [Documentation]    An elos client tries to publish a normal event and Succeeds

    ${PUBLISH_TIME}    Get Elos Event Publish Time Threshold

    Set Test Variable    ${PUBLISH_TIME}

    ${rc}    Execute And Log Based On User Permissions
    ...    elosc -p '{"messageCode": 150}'
    ...    ${RETURN_RC}
    Executable Returns No Errors    ${rc}    Client unable to publish normal event

A Security Event Is Published
    [Documentation]    Attempt to publish a blacklisted event will lead to a security event
    ...    to be published if client is unauthorized.

    ${stdout}    ${rc}    Execute And Log
    ...    elosc -f ".event.messageCode 8007 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND"
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Blacklisted event not filtered out by blacklist filter

Event Is Published
    [Documentation]    Event not blacklisted will be published.

    ${stdout}    ${rc}    Execute And Log
    ...    elosc -f ".event.messageCode 150 EQ .event.date.tv_sec ${PUBLISH_TIME} GE AND"
    ...    ${RETURN_STDOUT}
    ...    ${RETURN_RC}
    Should Contain    ${stdout}    150
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter

Reset Elosd Config
    [Documentation]    reset elosd config to default during test teardown.

    Stop Elosd
    Wait For Elosd To Stop
    Cleanup Template Config
    Start Elosd
    Wait Till Elosd Is Started
