*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check simple blacklist filter

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/TemplateConfig.py
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${BLACKLIST_FILTER}     .event.messageCode 2010 EQ
@{PROCESS_FILTER}       1 0 EQ    # all processes are unauthorized


*** Test Cases ***
01_Test_Simple_Blacklist_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Unauthorized process cannot publish a blacklisted event

    Given A Simple Blacklist Filter Is Set
    When Unauthorized Process Tries To Publish A Blacklisted Event
    Then A Security Event Is Published
    [Teardown]    Reset Elosd Config

02_Test_Simple_Blacklist_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Unauthorized process can publish events not blacklisted

    Given A Simple Blacklist Filter Is Set
    When Unauthorized Process Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
A Simple Blacklist Filter Is Set
    [Documentation]    Set a simple blacklist filter in config

    Ensure Elosd Is Stopped
    ${Config}    Default Config Core
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.EventBlacklist    ${BLACKLIST_FILTER}
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.authorizedProcesses    ${PROCESS_FILTER}
    Set Config From Template    &{Config}
    Ensure Elosd Is Started

Unauthorized Process Tries To Publish A Blacklisted Event
    [Documentation]    An elos client tries to publish a black listed event and fails

    ${output}    ${error}    ${rc}    Publish '{"messageCode": 2010}'
    Executable Returns An Error    ${rc}

Unauthorized Process Tries To Publish A Normal Event
    [Documentation]    An elos client tries to publish a normal event and Succeeds

    ${output}    ${error}    ${rc}    Publish '{"messageCode": 150}'
    Executable Returns No Errors    ${rc}    Unauthorized Process unable to publish normal event

A Security Event Is Published
    [Documentation]    Attempt to publish a blacklisted event will lead to a security event
    ...    to be published if client is unauthorized.

    ${stdout}    ${stderror}    ${rc}    Find Events Matching '.event.messageCode 8007 EQ'
    Should Contain    ${stdout}    2010
    Executable Returns No Errors    ${rc}    Blacklisted event not filtered out by blacklist filter

Event Is Published
    [Documentation]    Event not blacklisted will be published.

    ${stdout}    ${stderror}     ${rc}     Find Events Matching '.event.messageCode 150 EQ'
    Should Contain    ${stdout}    150
    Executable Returns No Errors    ${rc}    Event not filtered out by blacklist filter
