*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check trigger authorized process filter multiple times.

Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource
Library             libraries/TemplateConfig.py
Library             libraries/ElosKeywords.py

Suite Setup         Run Keywords    Connect To Target And Log In
...                 AND             Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${BLACKLIST_FILTER}             .event.messageCode 2010 EQ
@{AUTHORIZED_PROCESS_FILTER}    .process.gid 0 EQ


*** Test Cases ***
01_Test_Trigger_Filter_Multiple_Times    # robocop: disable=not-capitalized-test-case-title
    [Documentation]    Blacklisted events can be published by multiple authorized processes

    Given A Simple Authorized Process Filter Is Set
    When Multiple Clients Try To Publish A Blacklisted Event
    Then Blacklisted Event Is Published
    [Teardown]    Reset Elosd Config


*** Keywords ***
A Simple Authorized Process Filter Is Set
    [Documentation]    Set a simple authorized process filter in config

    Ensure Elosd Is Stopped
    Set Config From Template
    ...    EventBlacklist=${BLACKLIST_FILTER}
    ...    authorizedProcesses=${AUTHORIZED_PROCESS_FILTER}
    Ensure Elosd Is Started

Multiple Clients Try To Publish A Blacklisted Event
    [Documentation]    run multiple clients to publish blacklisted filters

    @{result}    Publish '{"messageCode": 2010}' '2' Times

Blacklisted Event Is Published
    [Documentation]    Blacklisted event will be published from authorized clients

    '2' Latest Events Matching '.event.messageCode 2010 EQ' Found
