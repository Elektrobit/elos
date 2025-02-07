*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check blacklist filter triggered multiple times

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
${CLIENTS}              2
${ERROR}                unauthorized publishing attempt


*** Test Cases ***
01_Test_Blacklist_Filter_Multiple_Times    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Check simple elos blacklist filter triggered multiple times.

    Given A Simple Blacklist Filter Is Set
    When Multiple Unauthorized Processes Try To Publish A Blacklisted Event
    Then A Security Event Is Published Every Time
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

Multiple Unauthorized Processes Try To Publish A Blacklisted Event
    [Documentation]    run multiple client to publish blacklisted filters

    ${output}    ${error}    ${rc}    Publish '{"messageCode": 2010}' '${CLIENTS}' Times
    Should Contain X Times    ${rc}    ${1}    ${2}

A Security Event Is Published Every Time
    [Documentation]    Attempt to publish a blacklisted event will lead to a security event
    ...    to be published if client is unauthorized.

    '${CLIENTS}' Latest Events Matching '.event.messageCode 8007 EQ' Found
