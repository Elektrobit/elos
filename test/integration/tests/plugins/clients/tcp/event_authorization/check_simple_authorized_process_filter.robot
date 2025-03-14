*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check simple authorized process filter

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/TemplateConfig.py
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${PUBLISH_TIME}                             0
${AUTHORIZE_ROOT}                           ${true}
${BLACKLIST_FILTER}                         .event.messageCode 2010 EQ
@{FILTER_TO_ALLOW_ROOT_PROCESSES}           .process.gid 0 EQ
@{FILTER_TO_ALLOW_NON_ROOT_PROCESSES}       .process.gid 0 NE


*** Test Cases ***
01_Test_Simple_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Authorized processes can publish blacklisted event.
    ...    All root processes are authorized.

    Given A Filter To Authorize Root Processes Is Set
    When Root Process Tries To Publish A Blacklisted Event
    Then Blacklisted Event Is Published
    [Teardown]    Reset Elosd Config

02_Test_Simple_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Authorized processes can publish normal event
    ...    All root processes are authorized.

    Given A Filter To Authorize Root Processes Is Set
    When Root Process Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config

03_Test_Simple_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Unauthorized processes can publish normal events
    ...    All root processes are unauthorized.

    Given A Filter To Authorize Non Root Processes Is Set
    When Root Process Tries To Publish A Normal Event
    Then Event Is Published
    [Teardown]    Reset Elosd Config

04_Test_Simple_Filter    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Unauthorized processes can not publish blacklisted events
    ...    All root processes are unauthorized.

    Given A Filter To Authorize Non Root Processes Is Set
    When Root Process Tries To Publish A Blacklisted Event
    Then A Security Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
A Filter To Authorize Root Processes Is Set
    [Documentation]    Set a simple authorized process filter to authorize all root processes

    Ensure Elosd Is Stopped
    ${Config}    Default Config Core
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.EventBlacklist    ${BLACKLIST_FILTER}
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.authorizedProcesses
    ...          ${FILTER_TO_ALLOW_ROOT_PROCESSES}
    Set Config From Template    &{Config}
    Ensure Elosd Is Started

A Filter To Authorize Non Root Processes Is Set
    [Documentation]    Set a simple authorized process filter to authorize all    non root processes

    Set Test Variable    ${AUTHORIZE_ROOT}    ${false}
    Ensure Elosd Is Stopped
    ${Config}    Default Config Core
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.EventBlacklist    ${BLACKLIST_FILTER}
    ${Config}    Update Value To Json
    ...          ${Config}    $..LocalTcpClient.Config.authorizedProcesses
    ...          ${FILTER_TO_ALLOW_NON_ROOT_PROCESSES}
    Set Config From Template    &{Config}
    Ensure Elosd Is Started

Root Process Tries To Publish A Blacklisted Event
    [Documentation]    A root process tries to publish a blacklisted event

    ${output}    ${error}     ${rc}    Publish '{"messageCode": 2010}'

    IF    ${AUTHORIZE_ROOT}
        Executable Returns No Errors
        ...    ${rc}
        ...    Authorized client unable to publish blacklisted event
    ELSE
        Executable Returns An Error    ${rc}
    END

Root Process Tries To Publish A Normal Event
    [Documentation]    A root process tries to publish a normal event

    ${output}    ${error}    ${rc}    Publish '{"messageCode": 150}'
    Executable Returns No Errors    ${rc}    Client unable to publish normal event

Blacklisted Event Is Published
    [Documentation]    Blacklisted event will be published from authorized clients

    Latest Events Matching '.event.messageCode 2010 EQ' Found

Event Is Published
    [Documentation]    Event not blacklisted will be published from authorized clients.

    Latest Events Matching '.event.messageCode 150 EQ' Found

A Security Event Is Published
    [Documentation]    Security event will be published if an unauthorized client tries to publish
    ...    a blacklisted event.

    Latest Events Matching '.event.messageCode 8007 EQ' Found
