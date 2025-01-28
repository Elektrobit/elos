*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check empty authorized process filter

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/TemplateConfig.py
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${BLACKLIST_FILTER}         .event.messageCode 2010 EQ
@{EMPTY_PROCESS_FILTERS}    ${EMPTY}    ${SPACE * 2}


*** Test Cases ***
01_Test_Empty_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Empty process filter renders a process unauthorized

    Given An Empty Authorized Process Filter Is Set
    When Client Tries To Publish A Blacklisted Event
    Then A Security Event Is Published
    [Teardown]    Reset Elosd Config

02_Test_Empty_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Normal events are published though process filter is empty

    Given An Empty Authorized Process Filter Is Set
    When Client Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
An Empty Authorized Process Filter Is Set
    [Documentation]    Set an empty authorized process filter in config

    Ensure Elosd Is Stopped
    ${Config}    Default Config Core
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.EventBlacklist    ${BLACKLIST_FILTER}
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.authorizedProcesses
    ...          ${EMPTY_PROCESS_FILTERS}
    Set Config From Template    &{Config}
    Ensure Elosd Is Started

Client Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${output}    ${error}     ${rc}    Publish '{"messageCode": 2010}'
    Executable Returns An Error    ${rc}

Client Tries To Publish A Normal Event
    [Documentation]    An elos client tries to publish a normal event and Succeeds

    ${output}    ${error}    ${rc}    Publish '{"messageCode": 150}'
    Executable Returns No Errors    ${rc}    Client unable to publish normal event

A Security Event Is Published
    [Documentation]    Attempt to publish a blacklisted event will lead to a security event
    ...    to be published if client is unauthorized.

    Latest Events Matching '.event.messageCode 8007 EQ' Found

Event Is Published
    [Documentation]    Event not blacklisted will be published.

    Latest Events Matching '.event.messageCode 150 EQ' Found
