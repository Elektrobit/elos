*** Comments ***
# SPDX-License-Identifier: MIT


*** Settings ***
Documentation       A test suite to check if an event is published
...                 when ever an oom killer is invoked.

Library             String
Library             SSHLibrary
Library             libraries/ElosKeywords.py
Library             libraries/ElosEventKeywords.py
Resource            resources/elosd-keywords.resource
Resource            resources/keywords.resource

Suite Setup         Run Keywords    Connect To Target And Log In
...                     AND    Ensure Elosd Is Started
Suite Teardown      Close All Connections


*** Variables ***
${OOM_EVENT_FILTER}         .event.messageCode 5020 EQ
${FILENAME}                 \/dev\/kmsg
${SEVERITY}                 3
${CLASSIFICATION}           1
${MESSAGE_CODE}             1111
${PAYLOAD}                  3,270,497530363,-;Out of memory: Killed process 115 (tail)


*** Test Cases ***
01_Test_OOM_Killer_Kernel_Event
    [Documentation]    When oom killer is invoked an event is
    ...    published.

    Given Elosd Is Running
    When An OOM Killer Is Invoked
    Then An OOM Event Is Published


*** Keywords ***
An OOM Killer Is Invoked
    [Documentation]    An oom killer    can be invoked by running
    ...    tail /dev/zero, but this is unreliable &
    ...    unsafe, so a similar kernel log event is
    ...    simulated via an elosc client

    ${OOM_EVENT}=    Get Default Event
    ${OOM_EVENT}=    Set '${FILENAME}' As '${OOM_EVENT}' File Name
    ${OOM_EVENT}=    Set '${SEVERITY}' As '${OOM_EVENT}' Severity
    ${OOM_EVENT}=    Set '${CLASSIFICATION}' As '${OOM_EVENT}' Classification
    ${OOM_EVENT}=    Set '${MESSAGE_CODE}' As '${OOM_EVENT}' Message Code
    ${OOM_EVENT}=    Set '${PAYLOAD}' As '${OOM_EVENT}' Payload

    ${OOM_EVENT_JSON}=   Get '${OOM_EVENT}' As Json String
    Publish '${OOM_EVENT_JSON}'

An OOM Event Is Published
    [Documentation]    OOM killer invoked event is published.

    Latest Events Matching '${OOM_EVENT_FILTER}' Found
